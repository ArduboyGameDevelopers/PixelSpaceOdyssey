//
//  character.c
//  PixelSpaceOdessey
//
//  Created by Alex Lementuev on 11/26/15.
//  Copyright © 2015 Space Madness. All rights reserved.
//

#include <avr/pgmspace.h>

#include "character.h"
#include "drawing.h"

#define FRAME_DELAY_MS 120

void CharacterSetAnimation(Character* character, const Animation* animation)
{
    assert(character);
    assert(animation);
    
    if (character->animation != animation)
    {
        character->animation = animation;
        character->animationEnded = false;
        character->frame = 0;
    }
}

void CharacterUpdate(Character* character, TimeInterval dt)
{
    if (!character->animationEnded)
    {
        character->frameTime += dt;
        if (character->frameTime >= FRAME_DELAY_MS)
        {
            character->frameTime = 0;
            ++character->frame;
            
            int frameCount = character->animation->frameCount;
            if (character->frame >= frameCount)
            {
                if (character->animation->looped)
                {
                    character->frame %= frameCount;
                }
                else
                {
                    character->frame = frameCount - 1;
                    character->animationEnded = true;
                    CharacterCallbackInvoke(character, CHARACTER_CALLBACK_ANIMATION_FINISHED);
                }
            }
        }
    }
}

void CharacterDraw(Character* character)
{
    assert(character);
    PgmPtr framePtr = AnimationGetFrame(character->animation, character->frame);

    DrawMode mode = DM_UNLIT;
    if (character->dir == DIR_LEFT)
    {
        mode |= DM_FLIP_X;
    }
    
    uint16_t frameWidth  = pgm_read_byte(framePtr + 2);
    uint16_t frameHeight = pgm_read_byte(framePtr + 3);
    int16_t drawX = W2S(character->x - DIV2(character->width));
    int16_t drawY = W2S(character->y - DIV2(character->height));
    
    if ((mode & DM_FLIP_X) == 0)
    {
        drawX += pgm_read_byte(framePtr);
    }
    else
    {
        int16_t extra = frameWidth & 7;
        if (extra != 0)
        {
            drawX += 8 - extra;
        }
    }
    if ((mode & DM_FLIP_Y) == 0)
    {
        drawY += pgm_read_byte(framePtr + 1);
    }
    
    PgmPtr imagePtr = framePtr + 4;
    drawImage(imagePtr, drawX, drawY, frameWidth, frameHeight, mode);
}

void CharacterCallbackInvoke(Character *character, CharacterCallbackType type, int16_t user1, int16_t user2)
{
    CharacterCallback callback = character->callback;
    if (callback)
    {
        callback(character, type, user1, user2);
    }
}

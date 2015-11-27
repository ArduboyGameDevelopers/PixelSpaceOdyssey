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
        character->frame = 0;
    }
}

void CharacterUpdateAnimation(Character* character, TimeInterval dt)
{
    character->frameTime += dt;
    if (character->frameTime >= FRAME_DELAY_MS)
    {
        character->frameTime = 0;
        character->frame = (character->frame + 1) % character->animation->frameCount;
    }
}

void CharacterDraw(Character* character, DrawMode mode)
{
    assert(character);
    PgmPtr framePtr = AnimationGetFrame(character->animation, character->frame);
    
    uint16_t frameWidth  = pgm_read_byte(framePtr + 2);
    uint16_t frameHeight = pgm_read_byte(framePtr + 3);
    int16_t drawX = WORLD_TO_SCREEN(character->x) - character->width / 2;
    int16_t drawY = WORLD_TO_SCREEN(character->y) - character->height;
    
    if ((mode & DM_FLIP_X) == 0)
    {
        drawX += pgm_read_byte(framePtr);

    }
    if ((mode & DM_FLIP_Y) == 0)
    {
        drawY += pgm_read_byte(framePtr + 1);
    }
    
    PgmPtr imagePtr = framePtr + 4;
    drawImage(imagePtr, drawX, drawY, frameWidth, frameHeight, mode);
}
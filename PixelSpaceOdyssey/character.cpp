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

#define FRAME_DELAY_MS 100

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
    
    uint8_t offsetX  = pgm_read_byte(framePtr);
    uint8_t offsetY  = pgm_read_byte(framePtr + 1);
    uint8_t width   = pgm_read_byte(framePtr + 2);
    uint8_t height  = pgm_read_byte(framePtr + 3);
    
    PgmPtr imagePtr = framePtr + 4;
    int16_t drawX = WORLD_TO_SCREEN(character->x) + offsetX;
    int16_t drawY = WORLD_TO_SCREEN(character->y) + offsetY;
    
    drawImage(imagePtr, drawX, drawY, width, height, mode);
}
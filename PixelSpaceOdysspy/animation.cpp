//
//  animation.c
//  PixelSpaceOdessey
//
//  Created by Alex Lementuev on 11/25/15.
//  Copyright © 2015 Space Madness. All rights reserved.
//

#include <avr/pgmspace.h>

#include "animation.h"
#include "graphics.h"

void AnimationPlayer::update(TimeInterval dt)
{
    _frameTime += dt;
    if (_frameTime >= 100)
    {
        _frameTime = 0;
        _frame = (_frame + 1) % frameCount();
    }
}

void AnimationPlayer::draw(Graphics* g, Coord x, Coord y, DrawMode mode)
{
    PgmPtr framePtr = getCurrentFrame();
    
    Coord offsetX  = pgm_read_byte(framePtr);
    uint8_t width   = pgm_read_byte(framePtr + 2);
    uint8_t height  = pgm_read_byte(framePtr + 3);
    
    PgmPtr imagePtr = framePtr + 4;
    g->drawImage(imagePtr, x + offsetX - width / 2, y - height, width, height, mode);
}

void AnimationPlayer::setAnimation(Animation* animation)
{
    if (_animation != animation)
    {
        _animation = animation;
        _frame = 0;
    }
}
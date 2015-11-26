//
//  character.c
//  PixelSpaceOdessey
//
//  Created by Alex Lementuev on 11/25/15.
//  Copyright © 2015 Space Madness. All rights reserved.
//

#include "character.h"

void Character::update(TimeInterval dt)
{
    _animationPlayer.update(dt);
}

void Character::draw(Graphics* g)
{
    _animationPlayer.draw(g, WORLD_TO_SCREEN(x), WORLD_TO_SCREEN(y));
}
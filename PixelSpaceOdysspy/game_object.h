//
//  game_object.h
//  PixelSpaceOdessey
//
//  Created by Alex Lementuev on 11/25/15.
//  Copyright © 2015 Space Madness. All rights reserved.
//

#ifndef game_object_h
#define game_object_h

#include "common.h"

class GameObject
{
public:
    int16_t x;
    int16_t y;
    
public:
    GameObject() :
        x(0),
        y(0)
    {
    }
    
public:
    virtual void update(TimeInterval dt) {}
    virtual void draw(Graphics* g) {}
};

#endif /* game_object_h */

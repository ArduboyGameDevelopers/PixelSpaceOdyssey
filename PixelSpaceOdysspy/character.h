//
//  character.h
//  PixelSpaceOdessey
//
//  Created by Alex Lementuev on 11/25/15.
//  Copyright © 2015 Space Madness. All rights reserved.
//

#ifndef character_h
#define character_h

#include "game_object.h"

#include "animation.h"

class Character : public GameObject
{
protected:
    AnimationPlayer _animationPlayer;
    
public:
    Character() :
        _animationPlayer()
    {
    }
    
public:
    virtual void update(TimeInterval dt);
    virtual void draw(Graphics* g);
};

#endif /* character_h */

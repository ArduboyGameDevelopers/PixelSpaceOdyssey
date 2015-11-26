//
//  player.h
//  PixelSpaceOdessey
//
//  Created by Alex Lementuev on 11/25/15.
//  Copyright © 2015 Space Madness. All rights reserved.
//

#ifndef player_h
#define player_h

#include "character.h"

class Player : public Character
{
private:
    bool    _jumping;
    bool    _crouching;
    int8_t  _moveDir;
    int16_t _jumpSpeed;
    
public:
    Player() :
        _jumping(false),
        _crouching(false),
        _moveDir(0),
        _jumpSpeed(0)
    {
    }
    
public:
    virtual void update(TimeInterval dt);
    
private:
    void setAnimation(int index);
};

#endif /* player_h */

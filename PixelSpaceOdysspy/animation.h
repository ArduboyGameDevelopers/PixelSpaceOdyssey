//
//  animation.h
//  PixelSpaceOdessey
//
//  Created by Alex Lementuev on 11/25/15.
//  Copyright © 2015 Space Madness. All rights reserved.
//

#ifndef animation_h
#define animation_h

#include "common.h"

class Animation
{
private:
    PgmPtr* _frames;
    uint8_t   _frameCount;
    
public:
    Animation(PgmPtr* frames, uint8_t frameCount) :
        _frames(frames),
        _frameCount(frameCount)
    {
    }
    
public:
    inline PgmPtr getFrame(int index) const
    {
        assert(index >= 0 && index < _frameCount);
        return _frames[index];
    }
    inline PgmPtr* frames()     const { return _frames; }
    inline uint8_t   frameCount() const { return _frameCount; }
};

class AnimationPlayer
{
private:
    Animation*   _animation;
    uint8_t      _frame;
    TimeInterval _frameTime;
    bool         _looped;
    
public:
    AnimationPlayer() :
        _animation(0),
        _frame(0),
        _frameTime(0),
        _looped(false)
    {
    }
    
public:
    void update(TimeInterval dt);
    void draw(Graphics* g, Coord x, Coord y, DrawMode mode = DM_UNLIT);
    
public:
    void setAnimation(Animation* animation);
    inline void setLooped(bool looped) { _looped = looped; }
    inline bool isLooped() const { return _looped; }
    
    inline uint8_t frameCount() const
    {
        assert(_animation);
        return _animation->frameCount();
    }
    
private:
    inline PgmPtr getCurrentFrame() const
    {
        assert(_animation);
        return _animation->getFrame(_frame);
    }
};

#endif /* animation_h */

//
//  animation.h
//  PixelSpaceOdessey
//
//  Created by Alex Lementuev on 11/26/15.
//  Copyright © 2015 Space Madness. All rights reserved.
//

#ifndef animation_h
#define animation_h

#include "common.h"

typedef struct _Animation {
    const PgmPtr* frames;
    uint8_t frameCount;
    bool looped;
} Animation;

inline Animation AnimationMake(const PgmPtr* frames, uint8_t frameCount, bool looped = false)
{
    Animation animation;
    animation.frames = frames;
    animation.frameCount = frameCount;
    animation.looped = looped;
    return animation;
}

inline PgmPtr AnimationGetFrame(const Animation* animation, uint8_t index)
{
    assert(animation);
    assert(index >= 0 && index < animation->frameCount);
    return animation->frames[index];
}

#endif /* animation_h */

//
//  common.h
//  PixelSpaceOdessey
//
//  Created by Alex Lementuev on 11/25/15.
//  Copyright © 2015 Space Madness. All rights reserved.
//

#ifndef common_h
#define common_h

#if EMULATOR
    #include <types.h>
#else
    #include <stdint.h>
#endif // EMULATOR

#include <assert.h>

#define WORLD_TO_SCREEN(X) ((X) >> 4)
#define SCREEN_TO_WORLD(X) ((X) << 4)

#define DM_UNLIT  1  // unlit pixel
#define DM_LIT    2  // lit pixel
#define DM_INVERT 4  // lit/unlit pixel
#define DM_FLIP_X 8
#define DM_FLIP_Y 16

typedef int16_t         Coord;
typedef uint8_t         DrawMode;
typedef const uint8_t*  PgmPtr;
typedef unsigned long   TimeInterval;

class Animation;
class AnimationPlayer;
class Image;
class Graphics;

#endif /* common_h */

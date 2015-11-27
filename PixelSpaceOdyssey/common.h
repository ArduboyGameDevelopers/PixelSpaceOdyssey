//
//  common.h
//  PixelSpaceOdessey
//
//  Created by Alex Lementuev on 11/26/15.
//  Copyright © 2015 Space Madness. All rights reserved.
//

#ifndef common_h
#define common_h

#if EMULATOR
    #include <stdio.h>
    #include <types.h>
    #include <stdint.h>

    #define DEBUG_LOG(format, ...) printf(format, __VA_ARGS__)
#else
    #define DEBUG_LOG(format, ...)
#endif

#include <assert.h>

#define WORLD_TO_SCREEN(X) ((X) >> 8)
#define SCREEN_TO_WORLD(X) ((X) << 8)

#define DM_UNLIT  1  // unlit pixel
#define DM_LIT    2  // lit pixel
#define DM_INVERT 4  // lit/unlit pixel
#define DM_FLIP_X 8
#define DM_FLIP_Y 16

typedef const unsigned char* PgmPtr;
typedef unsigned long TimeInterval;
typedef uint8_t DrawMode;

#endif /* common_h */

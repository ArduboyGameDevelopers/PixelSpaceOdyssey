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

    #define DEBUG_LOG(format, ...) printf(format, __VA_ARGS__)
    #define WEAK_CONST
#else
    #include <Arduino.h>

    #define DEBUG_LOG(format, ...)
    #define WEAK_CONST const
#endif

#include <assert.h>
#include <stdint.h>

#define DIV2(X) ((X) >> 1)
#define MUL2(X) ((X) << 1)

#define W2S(X) ((X) >> 4)
#define S2W(X) ((X) << 4)

#define DM_UNLIT  1  // unlit pixel
#define DM_LIT    2  // lit pixel
#define DM_INVERT 4  // lit/unlit pixel
#define DM_FLIP_X 8
#define DM_FLIP_Y 16

typedef WEAK_CONST unsigned char* PgmPtr;
typedef int32_t TimeInterval;
typedef uint8_t DrawMode;

#endif /* common_h */

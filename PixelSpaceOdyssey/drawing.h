//
//  drawing.h
//  PixelSpaceOdessey
//
//  Created by Alex Lementuev on 11/26/15.
//  Copyright © 2015 Space Madness. All rights reserved.
//

#ifndef drawing_h
#define drawing_h

#include <avr/pgmspace.h>

#include "common.h"

#define BUFFER_WIDTH 128
#define BUFFER_HEIGHT 64

extern int16_t drawTransX;
extern int16_t drawTransY;

class Arduboy;

void drawInit(Arduboy *arduboy);
void drawImage(PgmPtr image, int16_t x, int16_t y, uint8_t w, uint8_t h, DrawMode mode);
void fillRect(int16_t x, int16_t y, uint8_t w, uint8_t h, uint8_t color);
void setPixel(int16_t x, int16_t y, uint8_t color);

inline void drawImagePtr(PgmPtr imagePtr, int16_t x, int16_t y, DrawMode mode)
{
    uint16_t w = pgm_read_byte(imagePtr);
    uint16_t h = pgm_read_byte(imagePtr + 1);
    drawImage(imagePtr + 2, x, y, w, h, mode);
}

#endif /* drawing_h */

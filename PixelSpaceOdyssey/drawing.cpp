//
//  drawing.c
//  PixelSpaceOdessey
//
//  Created by Alex Lementuev on 11/26/15.
//  Copyright © 2015 Space Madness. All rights reserved.
//

#include <avr/pgmspace.h>

#include "drawing.h"

#define BUFFER_WIDTH 128
#define BUFFER_HEIGHT 64

static uint8_t* sBuffer;

int16_t drawTransX = 0;
int16_t drawTransY = 0;

void drawInit(unsigned char *buffer)
{
    sBuffer = buffer;
}

void drawImage(PgmPtr image, int16_t x, int16_t y, uint8_t w, uint8_t h, DrawMode mode)
{
    x += drawTransX;
    y += drawTransY;
    
    // no need to dar at all of we're offscreen
    if (x + w < 0 || x > BUFFER_WIDTH - 1 || y + h < 0 || y > BUFFER_HEIGHT - 1)
        return;
    
    int yOffset = abs(y) % 8;
    int sRow = y / 8;
    if (y < 0)
    {
        sRow--;
        yOffset = 8 - yOffset;
    }
    int rows = h / 8;
    if (h % 8 != 0) rows++;
    
    int8_t k1 = 0, k2 = 1;
    
    if (mode & DM_FLIP_X)
    {
        k1 = 1;
        k2 = -1;
    }
    
    for (int a = 0; a < rows; a++)
    {
        int bRow = sRow + a;
        if (bRow > (BUFFER_HEIGHT / 8) - 1) break;
        if (bRow > -2)
        {
            for (int iCol = 0; iCol<w; iCol++)
            {
                if (iCol + x > (BUFFER_WIDTH-1)) break;
                if (iCol + x >= 0)
                {
                    if (bRow >= 0)
                    {
                        if      (mode & DM_LIT)   sBuffer[ (bRow*BUFFER_WIDTH) + k1 * (w - 1) + x + k2 * iCol ] |= pgm_read_byte(image+(a*w)+iCol) << yOffset;
                        else if (mode & DM_UNLIT) sBuffer[ (bRow*BUFFER_WIDTH) + k1 * (w - 1) + x + k2 * iCol ] &= ~(pgm_read_byte(image+(a*w)+iCol) << yOffset);
                        else                      sBuffer[ (bRow*BUFFER_WIDTH) + k1 * (w - 1) + x + k2 * iCol ] ^= pgm_read_byte(image+(a*w)+iCol) << yOffset;
                    }
                    if (yOffset && bRow<(BUFFER_HEIGHT/8)-1 && bRow > -2)
                    {
                        if      (mode & DM_LIT)   sBuffer[ ((bRow+1)*BUFFER_WIDTH) + k1 * (w - 1) + x + k2 * iCol ] |= pgm_read_byte(image+(a*w)+iCol) >> (8-yOffset);
                        else if (mode & DM_UNLIT) sBuffer[ ((bRow+1)*BUFFER_WIDTH) + k1 * (w - 1) + x + k2 * iCol ] &= ~(pgm_read_byte(image+(a*w)+iCol) >> (8-yOffset));
                        else                      sBuffer[ ((bRow+1)*BUFFER_WIDTH) + k1 * (w - 1) + x + k2 * iCol ] ^= pgm_read_byte(image+(a*w)+iCol) >> (8-yOffset);
                    }
                }
            }
        }
    }
}
//
//  graphics.c
//  PixelSpaceOdessey
//
//  Created by Alex Lementuev on 11/25/15.
//  Copyright © 2015 Space Madness. All rights reserved.
//

#include <avr/pgmspace.h>

#include "graphics.h"
#include "image.h"

void Graphics::drawImage(const Image& image, Coord x, Coord y, DrawMode mode)
{
    drawImage(image.imagePtr(), x, y, image.width(), image.height());
}

void Graphics::drawImage(PgmPtr image, Coord x, Coord y, uint8_t w, uint8_t h, DrawMode mode)
{
    x += _tx;
    y += _ty;
    
    // no need to dar at all of we're offscreen
    if (x + w < 0 || x > _width - 1 || y + h < 0 || y > _height - 1)
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
        if (bRow > (_height / 8) - 1) break;
        if (bRow > -2)
        {
            for (int iCol = 0; iCol<w; iCol++)
            {
                if (iCol + x > (_width-1)) break;
                if (iCol + x >= 0)
                {
                    if (bRow >= 0)
                    {
                        if      (mode & DM_LIT)   _buffer[ (bRow*_width) + k1 * (w - 1) + x + k2 * iCol ] |= pgm_read_byte(image+(a*w)+iCol) << yOffset;
                        else if (mode & DM_UNLIT) _buffer[ (bRow*_width) + k1 * (w - 1) + x + k2 * iCol ] &= ~(pgm_read_byte(image+(a*w)+iCol) << yOffset);
                        else                      _buffer[ (bRow*_width) + k1 * (w - 1) + x + k2 * iCol ] ^= pgm_read_byte(image+(a*w)+iCol) << yOffset;
                    }
                    if (yOffset && bRow<(_height/8)-1 && bRow > -2)
                    {
                        if      (mode & DM_LIT)   _buffer[ ((bRow+1)*_width) + k1 * (w - 1) + x + k2 * iCol ] |= pgm_read_byte(image+(a*w)+iCol) >> (8-yOffset);
                        else if (mode & DM_UNLIT) _buffer[ ((bRow+1)*_width) + k1 * (w - 1) + x + k2 * iCol ] &= ~(pgm_read_byte(image+(a*w)+iCol) >> (8-yOffset));
                        else                      _buffer[ ((bRow+1)*_width) + k1 * (w - 1) + x + k2 * iCol ] ^= pgm_read_byte(image+(a*w)+iCol) >> (8-yOffset);
                    }
                }
            }
        }
    }
}
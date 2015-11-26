//
//  graphics.h
//  PixelSpaceOdessey
//
//  Created by Alex Lementuev on 11/25/15.
//  Copyright © 2015 Space Madness. All rights reserved.
//

#ifndef graphics_h
#define graphics_h

#include "common.h"

class Graphics
{
private:
    uint8_t* _buffer;
    uint8_t  _width;
    uint8_t  _height;
    Coord    _tx;
    Coord    _ty;
    
public:
    Graphics(uint8_t* buffer, uint8_t width, uint8_t height) :
        _buffer(buffer),
        _width(width),
        _height(height),
        _tx(0),
        _ty(0)
    {
    }
    
public:
    void drawImage(const Image& image, Coord x, Coord y, DrawMode mode = 0);
    void drawImage(PgmPtr image, Coord x, Coord y, uint8_t w, uint8_t h, DrawMode mode = 0);
    inline void translate(Coord x, Coord y) { _tx += x; _ty += y; }
};

#endif /* graphics_h */

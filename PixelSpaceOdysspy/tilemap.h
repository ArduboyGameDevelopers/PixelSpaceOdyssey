//
//  tilemap.h
//  PixelSpaceOdessey
//
//  Created by Alex Lementuev on 11/25/15.
//  Copyright © 2015 Space Madness. All rights reserved.
//

#ifndef tilemap_h
#define tilemap_h

#include "common.h"

class TileMap
{
private:
    PgmPtr*  _tiles;
    uint8_t  _tileCount;
    uint8_t* _indices;
    uint8_t  _width;
    uint8_t  _height;
    
public:
    TileMap(PgmPtr* tiles,
            uint8_t tileCount,
            uint8_t* indices,
            uint8_t width,
            uint8_t height) :
        _tiles(tiles),
        _tileCount(tileCount),
        _indices(indices),
        _width(width),
        _height(height)
    {
    }
    
public:
    void draw(Graphics* g, Coord x, Coord y);
    
public:
    uint16_t pixelWidth();
    uint16_t pixelHeight();
};

#endif /* tilemap_h */

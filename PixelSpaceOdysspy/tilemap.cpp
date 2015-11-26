//
//  tilemap.c
//  PixelSpaceOdessey
//
//  Created by Alex Lementuev on 11/25/15.
//  Copyright © 2015 Space Madness. All rights reserved.
//

#include "tilemap.h"
#include "graphics.h"

#define TILE_WIDTH 8
#define TILE_HEIGHT 8

void TileMap::draw(Graphics* g, Coord sx, Coord sy)
{
    g->translate(sx, sy);
    
    int16_t x = 0, y = 0;
    uint16_t index = 0;
    for (int ty = 0; ty < _height; ++ty)
    {
        for (int tx = 0; tx < _width; ++tx)
        {
            int tileIndex = _indices[index];
            g->drawImage(_tiles[tileIndex], x, y, TILE_WIDTH, TILE_HEIGHT, DM_UNLIT);
            x += TILE_WIDTH;
            ++index;
        }
        x = 0;
        y += TILE_HEIGHT;
    }
    
    g->translate(-sx, -sy);
}

uint16_t TileMap::pixelWidth()
{
    return _width * TILE_WIDTH;
}

uint16_t TileMap::pixelHeight()
{
    return _height * TILE_HEIGHT;
}
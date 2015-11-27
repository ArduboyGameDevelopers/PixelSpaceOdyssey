//
//  tilemap.c
//  PixelSpaceOdessey
//
//  Created by Alex Lementuev on 11/26/15.
//  Copyright © 2015 Space Madness. All rights reserved.
//

#include <avr/pgmspace.h>

#include "tilemap.h"

#include "tiles.h"
#include "drawing.h"

void TileMapDraw(const TileMap* tileMap, int16_t x, int16_t y)
{
    uint8_t width = tileMap->width;
    uint8_t height = tileMap->heigth;
    const uint8_t* indices = tileMap->indices;
    
    uint16_t index = 0;
    for (int ty = 0; ty < height; ++ty)
    {
        for (int tx = 0; tx < width; ++tx)
        {
            uint8_t tileIndex = pgm_read_byte(indices + index);
            drawImage(getTile(tileIndex), x, y, TILE_WIDTH, TILE_HEIGHT, DM_UNLIT);
            x += TILE_WIDTH;
            ++index;
        }
        x = 0;
        y += TILE_HEIGHT;
    }
}

uint8_t TileMapGetIndex(const TileMap* tileMap, int16_t x, int16_t y)
{
    int16_t tx = x / TILE_WIDTH;
    int16_t ty = y / TILE_HEIGHT;
    
    uint8_t width = tileMap->width;
    uint8_t height = tileMap->heigth;
    
    if (tx >= 0 && tx < width && ty >= 0 && ty < height)
    {
        return ty * width + tx;
    }
    
    return 0;
}

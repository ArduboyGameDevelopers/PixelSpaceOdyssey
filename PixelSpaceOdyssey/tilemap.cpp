//
//  tilemap.c
//  PixelSpaceOdessey
//
//  Created by Alex Lementuev on 11/26/15.
//  Copyright © 2015 Space Madness. All rights reserved.
//

#include <avr/pgmspace.h>

#include "tilemap.h"
#include "drawing.h"

#define THIN_TILES_COUNT 13

void TileMapDraw(const TileMap* tileMap, int16_t x, int16_t y)
{
    const PgmPtr* tiles = tileMap->tiles;
    const uint8_t* indices = tileMap->indices;
    uint8_t rows = tileMap->rows;
    uint8_t cols = tileMap->cols;
    
    uint16_t index = 0;
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            uint8_t tileIndex = pgm_read_byte(indices + index);
            if (tileIndex > 0)
            {
                drawImage(tiles[tileIndex - 1], x, y, TILE_WIDTH_PX, TILE_HEIGHT_PX, DM_UNLIT);
            }
            x += TILE_WIDTH_PX;
            ++index;
        }
        x = 0;
        y += TILE_HEIGHT_PX;
    }
}

bool TileMapGetTile(const TileMap* tileMap, int16_t x, int16_t y, Tile* tile)
{
    int16_t i = W2S(y) / TILE_HEIGHT_PX;
    int16_t j = W2S(x) / TILE_WIDTH_PX;
    
    uint8_t rows = tileMap->rows;
    uint8_t cols = tileMap->cols;
    
    if (j >= 0 && j < cols && i >= 0 && i < rows)
    {
        uint8_t index = pgm_read_byte(tileMap->indices + i * cols + j);
        if (index <= THIN_TILES_COUNT)
        {
            return false;
        }
        
        tile->index = index;
        tile->x = j * TILE_WIDTH + TILE_WIDTH_HALF;
        tile->y = i * TILE_HEIGHT + TILE_HEIGHT_HALF;
        return true;
    }
    
    return false;
}

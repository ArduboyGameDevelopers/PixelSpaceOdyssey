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
#include "game.h"

static const int VISIBLE_ROWS = CAM_HEIGHT_PX / TILE_HEIGHT_PX;
static const int VISIBLE_COLS = CAM_WIDTH_PX / TILE_WIDTH_PX;

void TileMapDraw(const TileMap* tileMap)
{
    const PgmPtr* tiles = tileMap->tiles;
    const uint8_t* indices = tileMap->indices;
    uint8_t rows = tileMap->rows;
    uint8_t cols = tileMap->cols;
    
    uint16_t startRow = -drawTransY / TILE_HEIGHT_PX;
    uint16_t startCol = -drawTransX / TILE_WIDTH_PX;
    int16_t x0 = startCol * TILE_WIDTH_PX;
    int16_t y0 = startRow * TILE_HEIGHT_PX;
    
    int x = x0, y = y0;
    for (int i = 0; i < rows; ++i)
    {
        int index = (startRow + i) * cols + startCol;
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
        x = x0;
        y += TILE_HEIGHT_PX;
    }
}

uint8_t TileMapGetTile(const TileMap* tileMap, int16_t x, int16_t y, Tile* tile)
{
    int16_t i = y / TILE_HEIGHT;
    int16_t j = x / TILE_WIDTH;
    
    uint8_t rows = tileMap->rows;
    uint8_t cols = tileMap->cols;
    
    if (j >= 0 && j < cols && i >= 0 && i < rows)
    {
        uint8_t index = pgm_read_byte(tileMap->indices + i * cols + j);
        if (TILE_IS_THIN(index))
        {
            return 0;
        }
        
        tile->index = index;
        tile->x = j * TILE_WIDTH + TILE_WIDTH_HALF;
        tile->y = i * TILE_HEIGHT + TILE_HEIGHT_HALF;
        
        return index;
    }
    
    return 0;
}

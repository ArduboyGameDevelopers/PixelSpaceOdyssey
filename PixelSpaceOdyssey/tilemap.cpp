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

static inline int16_t ToTileMapIndex(const TileMap &tileMap, uint8_t tileIndex)
{
    if (TILE_IS_ITEM(tileIndex))
    {
        return TileItemIsPicked(tileMap, tileIndex) ? 0 : (TILE_ITEM_MIN + TileItemGetType(tileIndex) - 1);
    }
    
    return tileIndex - 1;
}

void TileMapDraw(const TileMap &tileMap)
{
    const PgmPtr* tiles = tileMap.tiles;
    const uint8_t* indices = tileMap.indices;
    uint8_t rows = tileMap.rows;
    uint8_t cols = tileMap.cols;
    
    uint16_t index = 0;
    int16_t x = 0;
    int16_t y = 0;
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            uint8_t indexData = pgm_read_byte(indices + index);
            int tileIndex = ToTileMapIndex(tileMap, indexData);
            if (tileIndex > 0)
            {
                drawImage(tiles[tileIndex], x, y, TILE_WIDTH_PX, TILE_HEIGHT_PX, DM_UNLIT);
            }
            x += TILE_WIDTH_PX;
            ++index;
        }
        x = 0;
        y += TILE_HEIGHT_PX;
    }
}

uint8_t TileMapGetTile(const TileMap &tileMap, int16_t x, int16_t y, Tile* tile)
{
    int16_t i = y / TILE_HEIGHT;
    int16_t j = x / TILE_WIDTH;
    
    uint8_t rows = tileMap.rows;
    uint8_t cols = tileMap.cols;
    
    if (j >= 0 && j < cols && i >= 0 && i < rows)
    {
        uint8_t index = pgm_read_byte(tileMap.indices + i * cols + j);
        if (TILE_IS_THIN(index) || (TILE_IS_ITEM(index) && TileItemIsPicked(tileMap, index)))
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

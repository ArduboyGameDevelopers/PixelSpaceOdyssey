//
//  tilemap.h
//  PixelSpaceOdessey
//
//  Created by Alex Lementuev on 11/26/15.
//  Copyright © 2015 Space Madness. All rights reserved.
//

#ifndef tilemap_h
#define tilemap_h

#include "common.h"

#define TILE_SLOPE_RIGHT 14
#define TILE_SLOPE_LEFT  15

const uint8_t TILE_WIDTH_PX     = 8;
const uint8_t TILE_HEIGHT_PX    = 8;
const uint8_t TILE_WIDTH        = S2W(TILE_WIDTH_PX);
const uint8_t TILE_HEIGHT       = S2W(TILE_HEIGHT_PX);
const uint8_t TILE_WIDTH_HALF   = TILE_WIDTH / 2;
const uint8_t TILE_HEIGHT_HALF  = TILE_HEIGHT / 2;

#define TILE_GET_LEFT(TILE)   ((TILE).x - TILE_WIDTH_HALF)
#define TILE_GET_RIGHT(TILE)  ((TILE).x + TILE_WIDTH_HALF)
#define TILE_GET_TOP(TILE)    ((TILE).y - TILE_HEIGHT_HALF)
#define TILE_GET_BOTTOM(TILE) ((TILE).y + TILE_HEIGHT_HALF)

#define TILE_GET_ROW(TILE) ((TILE).y / TILE_HEIGHT)
#define TILE_GET_COL(TILE) ((TILE).x / TILE_WIDTH)

#define TILE_INDEX_IS_SLOPE(INDEX) ((INDEX) == TILE_SLOPE_RIGHT || (INDEX) == TILE_SLOPE_LEFT)

#define TILEMAP_GET_WIDTH(TILEMAP) ((TILEMAP).cols * TILE_WIDTH)
#define TILEMAP_GET_HEIGHT(TILEMAP) ((TILEMAP).rows * TILE_HEIGHT)

typedef struct _Tile {
    int16_t x;
    int16_t y;
    uint8_t index;
} Tile;

typedef struct _TileMap {
    WEAK_CONST PgmPtr *tiles;
    WEAK_CONST uint8_t *indices;
    uint8_t rows;
    uint8_t cols;
} TileMap;

inline TileMap TileMapMake(WEAK_CONST PgmPtr *tiles, WEAK_CONST uint8_t *indices, uint8_t rows, uint8_t cols)
{
    TileMap tileMap;
    tileMap.tiles   = tiles;
    tileMap.indices = indices;
    tileMap.cols    = cols;
    tileMap.rows    = rows;
    return tileMap;
}

void TileMapDraw(const TileMap *tileMap, int16_t x, int16_t y);
uint8_t TileMapGetTile(const TileMap *tileMap, int16_t x, int16_t y, Tile* tile);

#endif /* tilemap_h */

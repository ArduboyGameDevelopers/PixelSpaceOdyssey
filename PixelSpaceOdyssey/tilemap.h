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

#define TILE_EXIT_MIN           23
#define TILE_EXIT_MAX           24
#define TILE_THIN_MAX           26
#define TILE_SLOPE_RIGHT_MIN    27
#define TILE_SLOPE_RIGHT_MAX    28
#define TILE_SLOPE_LEFT_MIN     29
#define TILE_SLOPE_LEFT_MAX     30
#define TILE_SOLID_MIN          31
#define TILE_SOLID_MAX          45
#define TILE_HAZARD_MIN         50
#define TILE_HAZARD_MAX         61
#define TILE_ITEM_MIN           62
#define TILE_ITEM_MAX           64

#define ITEM_TYPE_BITS_COUNT  2
#define ITEM_TYPE_MASK 0x3 // 00000011

#define ITEM_TYPE_HEALTH    0
#define ITEM_TYPE_AMMO      1
#define ITEM_TYPE_ROCK      2
#define ITEM_TYPE_MAX       3

#define ITEM_INDEX_BITS_COUNT 5
#define ITEM_INDEX_MASK 0x1f // 00011111
#define ITEM_INDEX_BITS_OFFSET ITEM_TYPE_BITS_COUNT
#define ITEM_INDEX_MAX 31

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

#define TILE_GET_ROW(Y) ((Y) / TILE_HEIGHT)
#define TILE_GET_COL(X) ((X) / TILE_WIDTH)

inline bool TILE_IS_EXIT(int16_t index)         { return index >= TILE_EXIT_MIN && index <= TILE_EXIT_MAX; }
inline bool TILE_IS_HAZARD(int16_t index)       { return index >= TILE_HAZARD_MIN && index <= TILE_HAZARD_MAX; }
inline bool TILE_IS_ITEM(int16_t index)         { return index & 0x80; }
inline bool TILE_IS_THIN(int16_t index)         { return index <= TILE_THIN_MAX; }
inline bool TILE_IS_SLOPE_LEFT(int16_t index)   { return index >= TILE_SLOPE_LEFT_MIN && index <= TILE_SLOPE_LEFT_MAX; }
inline bool TILE_IS_SLOPE_RIGHT(int16_t index)  { return index >= TILE_SLOPE_RIGHT_MIN && index <= TILE_SLOPE_RIGHT_MAX; }

#define TILEMAP_GET_WIDTH(TILEMAP)  ((TILEMAP).cols * TILE_WIDTH)
#define TILEMAP_GET_HEIGHT(TILEMAP) ((TILEMAP).rows * TILE_HEIGHT)

typedef struct _Tile {
    int16_t x;
    int16_t y;
    uint8_t index;
} Tile;

typedef struct _TileMap {
    WEAK_CONST PgmPtr *tiles;
    WEAK_CONST uint8_t *indices;
    uint32_t items;
    uint8_t rows;
    uint8_t cols;
} TileMap;

inline int TileMapGetIndexCount(const TileMap &tileMap)
{
    return tileMap.rows * tileMap.cols;
}

inline TileMap TileMapMake(WEAK_CONST PgmPtr *tiles, WEAK_CONST uint8_t *indices, uint8_t rows, uint8_t cols)
{
    TileMap tileMap;
    tileMap.tiles   = tiles;
    tileMap.indices = indices;
    tileMap.items = 0;
    tileMap.cols    = cols;
    tileMap.rows    = rows;
    return tileMap;
}

void TileMapDraw(const TileMap &tileMap);
uint8_t TileMapGetTile(const TileMap &tileMap, int16_t x, int16_t y, Tile* tile);

inline uint8_t TileItemGetIndex(uint8_t item)
{
    return (item >> ITEM_INDEX_BITS_OFFSET) & ITEM_INDEX_MASK;
}

inline uint8_t TileItemGetType(uint8_t item)
{
    return item & ITEM_TYPE_MASK;
}

inline bool TileItemIsPicked(const TileMap &tileMap, uint8_t item)
{
    int index = TileItemGetIndex(item);
    return tileMap.items & (1 << index);
}

inline void TileItemSetPicked(TileMap &tileMap, uint8_t item, bool picked)
{
    int index = TileItemGetIndex(item);
    if (picked)
    {
        tileMap.items |= 1 << index;
    }
    else
    {
        tileMap.items &= ~(1 << index);
    }
}

#if EMULATOR
inline uint8_t TileItemSetIndex(uint8_t item, int index)
{
    assert(index >= 0 && index <= ITEM_INDEX_MAX);
    return (item & 0x83 /* 10000011 */) | (index << ITEM_INDEX_BITS_OFFSET);
}

inline uint8_t TileItemSetType(uint8_t item, int type)
{
    assert(type >= 0 && type <= ITEM_TYPE_MAX);
    return (item & 0xfc /* 11111100 */) | type;
}
#endif /* EMULATOR */

#endif /* tilemap_h */

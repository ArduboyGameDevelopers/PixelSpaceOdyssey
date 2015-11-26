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

typedef struct _TileMap {
    const uint8_t* indices;
    uint8_t  width;
    uint8_t  heigth;
} TileMap;

inline TileMap TileMapMake(const uint8_t* indices, uint8_t width, uint8_t height)
{
    TileMap tileMap;
    tileMap.indices = indices;
    tileMap.width   = width;
    tileMap.heigth  = height;
    return tileMap;
}

void TileMapDraw(const TileMap* tileMap, int16_t x, int16_t y);

#endif /* tilemap_h */

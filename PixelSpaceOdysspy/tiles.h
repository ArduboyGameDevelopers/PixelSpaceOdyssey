//
//  tiles.h
//  PixelSpaceOdessey
//
//  Created by Alex Lementuev on 11/26/15.
//  Copyright © 2015 Space Madness. All rights reserved.
//

#ifndef tiles_h
#define tiles_h

#include <stdio.h>

#include "tilemap.h"

#define TILE_WIDTH  8
#define TILE_HEIGHT 8
#define TILES_COUNT 19

extern PgmPtr const tiles[];

TileMap tileMap1();

inline PgmPtr getTile(int index)
{
    assert(index >= 0 && index < TILES_COUNT);
    return tiles[index];
}

#endif /* tiles_h */

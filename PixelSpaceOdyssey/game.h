//
//  game.h
//  PixelSpaceOdessey
//
//  Created by Alex Lementuev on 11/26/15.
//  Copyright © 2015 Space Madness. All rights reserved.
//

#ifndef game_h
#define game_h

#include "common.h"
#include "tilemap.h"

extern TileMap tileMap;
extern int16_t camX;
extern int16_t camY;
extern uint16_t tileMapWidth;
extern uint16_t tileMapHeight;

extern const int16_t CAM_WIDTH;
extern const int16_t CAM_HEIGHT;
extern const int16_t CAM_WIDTH_HALF;
extern const int16_t CAM_HEIGHT_HALF;
extern const int16_t CAM_RANGE_X;
extern const int16_t CAM_RANGE_Y;

#endif /* game_h */

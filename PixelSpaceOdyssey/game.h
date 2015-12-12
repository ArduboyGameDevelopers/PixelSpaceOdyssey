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
#include "character.h"
#include "tilemap.h"
#include "enemies.h"

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

extern Character player;
extern Character *enemies;
extern uint8_t enemiesCount;

extern void initEnemies(uint8_t enemiesCount);
extern void addEnemy(const Character &enemy, int16_t x, int16_t y);

inline uint16_t playerDistanceSqr(const Character *character)
{
    int16_t dx = W2S(character->x - player.x);
    int16_t dy = W2S(character->y - player.y);
    return dx * dx + dy * dy;
}

#endif /* game_h */

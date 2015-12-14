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
extern const int8_t  WALK_SPEED;

extern Character player;

/* Enemy AI will target this position to create some delay */
extern int16_t playerLastSeenX;
extern int16_t playerLastSeenY;

extern Character *enemies;
extern uint8_t enemiesCount;

extern void initEnemies(uint8_t enemiesCount);
extern void addEnemy(const Character &enemy, int16_t x, int16_t y);

inline uint16_t playerDistanceHor(const Character *character)
{
    return W2S(playerLastSeenX - character->x);
}

inline uint16_t playerDistanceSqr(const Character *character)
{
    int16_t dx = W2S(character->x - playerLastSeenX);
    int16_t dy = W2S(character->y - playerLastSeenY);
    return dx * dx + dy * dy;
}

inline uint8_t getTile(int16_t x, int16_t y, Tile *tilePtr) { return TileMapGetTile(&tileMap, x, y, tilePtr); }
inline bool getSolidTile(int16_t x, int16_t y, Tile *tilePtr) { return TileMapGetTile(&tileMap, x, y, tilePtr) > 15; }

#endif /* game_h */

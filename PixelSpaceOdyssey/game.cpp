//
//  game.c
//  PixelSpaceOdessey
//
//  Created by Alex Lementuev on 11/26/15.
//  Copyright © 2015 Space Madness. All rights reserved.
//

#include "game.h"
#include "bridge.h"
#include "common.h"
#include "drawing.h"

#include "Arduboy.h"

#include "character.h"
#include "tilemap.h"

#include "tiles_tb_caverns.h"
#include "tiles_lair_01.h"
#include "enemies.h"
#include "hud.h"

const int8_t WALK_SPEED         = 15;

const int16_t CAM_WIDTH_PX      = 128;
const int16_t CAM_HEIGHT_PX     = 64-8; // subtract hud
const int16_t CAM_WIDTH         = S2W(CAM_WIDTH_PX);
const int16_t CAM_HEIGHT        = S2W(CAM_HEIGHT_PX);
const int16_t CAM_WIDTH_HALF    = CAM_WIDTH / 2;
const int16_t CAM_HEIGHT_HALF   = CAM_HEIGHT / 2;
const int16_t CAM_RANGE_X       = S2W(30);
const int16_t CAM_RANGE_Y       = S2W(15);

static Arduboy display;

int16_t playerLastSeenX;
int16_t playerLastSeenY;

Character *enemies = NULL;
uint8_t enemiesCount = 0;

TileMap tileMap = TileMapMake(TILES_TB_CAVERNS, INDICES_LAIR_01, TILEMAP_LAIR_01_HEIGHT, TILEMAP_LAIR_01_WIDTH);

/* Tile map size in world space */
uint16_t tileMapWidth;
uint16_t tileMapHeight;

/* Camera center pos in world space */
int16_t camX;
int16_t camY;

static TimeInterval lastFrameTime;

static void enemiesUpdate(TimeInterval dt);
static void enemiesDraw();

static inline void tilemapDraw()
{
    TileMapDraw(&tileMap);
}

void startGame()
{
    display.start();
    drawInit(&display);
    
    initGame();
}

void initGame()
{
    createPlayer();
    player.x = S2W(62);
    player.y = S2W(28);
    
    playerLastSeenX = player.x;
    playerLastSeenY = player.y;
    
    tileMapWidth = TILEMAP_GET_WIDTH(tileMap);
    tileMapHeight = TILEMAP_GET_HEIGHT(tileMap);
    
    camX = CAM_WIDTH_HALF;
    camY = CAM_HEIGHT_HALF;
    lastFrameTime = millis();
}

void updateGame()
{
    TimeInterval dt = millis() - lastFrameTime;
    lastFrameTime = millis();
    
    DispatchUpdate(dt);
    playerUpdate(dt);
    enemiesUpdate(dt);
}

void drawGame()
{
    // if (millis() - lastDrawTime < 33) return;
    // lastDrawTime = millis();
    
    display.fillRect(0, 0, WIDTH, HEIGHT, WHITE);
    
    drawTransX = -W2S(camX - CAM_WIDTH_HALF);
    drawTransY = -W2S(camY - CAM_HEIGHT_HALF);
    
    tilemapDraw();
    playerDraw();
    enemiesDraw();
    
    drawHud();
    
    display.display();
}

#pragma mark -
#pragma mark Enemies

void initEnemies(uint8_t count)
{
    free(enemies);
    enemies = (Character *) malloc(count * sizeof(Character));
    enemiesCount = 0;
}

void addEnemy(const Character &enemy, int16_t x, int16_t y)
{
    enemies[enemiesCount] = enemy;
    enemies[enemiesCount].x = x;
    enemies[enemiesCount].y = y;
    UpdateConstraints(&enemies[enemiesCount]);
    ++enemiesCount;
}

void enemiesUpdate(TimeInterval dt)
{
    for (int i = 0; i < enemiesCount; ++i)
    {
        EnemyUpdate(&enemies[i], dt);
    }
}

void enemiesDraw()
{
    for (int i = 0; i < enemiesCount; ++i)
    {
        CharacterDraw(&enemies[i]);
    }
}

#pragma mark -
#pragma mark Helpers

bool buttonPressed(int button)
{
    return display.pressed(button);
}

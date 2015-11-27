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

#include "player_animations.h"

#include "tiles_lair_01.h"

#define RUN_BUTTON  A_BUTTON
#define JUMP_BUTTON B_BUTTON

#define GRAVITY 75
#define FLOOR   416

#define JUMP_SPEED -1000
#define WALK_SPEED 150

#define PLAYER_WIDTH  2048 /* S2W(8) */
#define PLAYER_HEIGHT 2048 /* S2W(8) */
#define PLAYER_HALF_WIDTH  1024 /* S2W(4) */
#define PLAYER_HALF_HEIGHT 1024 /* S2W(4) */

#define CAM_WIDTH       32768 /* S2W(128) */
#define CAM_HEIGHT      16384 /* S2W(64/2) */
#define CAM_WIDTH_HALF  16384 /* S2W(128/2) */
#define CAM_HEIGHT_HALF 8192  /* S2W(64/2) */
#define CAM_RANGE_X     7680  /* S2W(30) */
#define CAM_RANGE_Y     3840  /* S2W(15) */

static Arduboy display;

static Character player = CharacterMake(PLAYER_WIDTH, PLAYER_HEIGHT);
static bool playerCrouching = false;
static bool playerJumping = false;
static bool jumpPressed = false;
static int16_t playerJumpSpeed = 0;

static TileMap tileMap = TileMapMake(TILES_LAIR_01, INDICES_LAIR_01, TILEMAP_LAIR_01_HEIGHT, TILEMAP_LAIR_01_WIDTH);

/* Tile map size in world space */
static uint16_t tileMapWidth = TILEMAP_GET_WIDTH(tileMap);
static uint16_t tileMapHeight = TILEMAP_GET_HEIGHT(tileMap);

/* Camera center pos in world space */
static int16_t camX;
static int16_t camY;

static TimeInterval lastFrameTime;

void updateInput();

void playerUpdate(TimeInterval dt);
void playerDraw();
void playerSetAnimation(int index);

void spiderUpdate(TimeInterval dt);
void spiderDraw();

void tilemapDraw();

void startGame()
{
    display.start();
    drawInit(display.getBuffer());
    
    player.x = S2W(12);
    player.y = S2W(36);
    
    camX = CAM_WIDTH_HALF;
    camY = CAM_HEIGHT_HALF;
    lastFrameTime = millis();
}

void updateGame()
{
    TimeInterval dt = millis() - lastFrameTime;
    lastFrameTime = millis();
    
    updateInput();
    playerUpdate(dt);
    spiderUpdate(dt);
}

void drawGame()
{
    display.fillRect(0, 0, WIDTH, HEIGHT, WHITE);
    
    drawTransX = -W2S(camX - CAM_WIDTH_HALF);
    drawTransY = -W2S(camY - CAM_HEIGHT_HALF);
    
    tilemapDraw();
    playerDraw();
    spiderDraw();
    
    display.display();
}

////////////////////////////////////////////////////////////////////
// Input

void updateInput()
{
//    if (arduboy.pressed(LEFT_BUTTON))
//    {
//        camX -= 2;
//        if (camX < 0) camX = 0;
//    }
//    else if (arduboy.pressed(RIGHT_BUTTON))
//    {
//        camX += 2;
//        if (camX > camXMax) camX = camXMax;
//    }
//    
//    if (arduboy.pressed(UP_BUTTON))
//    {
//        camY -= 2;
//        if (camY < 0) camY = 0;
//    }
//    else if (arduboy.pressed(DOWN_BUTTON))
//    {
//        camY += 2;
//        if (camY > camYMax) camY = camYMax;
//    }
//    
//    if (arduboy.pressed(A_BUTTON) || arduboy.pressed(B_BUTTON))
//    {
//        if (spider.state() == SPIDER_SLEEP)
//        {
//            spider.setState(SPIDER_AWAKEN);
//        }
//    }
}

////////////////////////////////////////////////////////////////////
// Player

#define GET_TILE(X,Y,T) TileMapGetTile(&tileMap, X, Y, &T)

#define PLAYER_TOP           (player.y - PLAYER_HALF_HEIGHT)
#define PLAYER_BOTTOM        (player.y + PLAYER_HALF_HEIGHT)
#define PLAYER_LEFT          (player.x - PLAYER_HALF_WIDTH + 1)
#define PLAYER_RIGHT         (player.x + PLAYER_HALF_WIDTH - 1)

inline void PLAYER_SET_TOP(int16_t y)    { player.y = y + PLAYER_HALF_HEIGHT; }
inline void PLAYER_SET_BOTTOM(int16_t y) { player.y = y - PLAYER_HALF_HEIGHT; }
inline void PLAYER_SET_LEFT(int16_t x)   { player.x = x + PLAYER_HALF_WIDTH; }
inline void PLAYER_SET_RIGHT(int16_t x)  { player.x = x - PLAYER_HALF_WIDTH; }

void playerHandleTileHorCollision(const Tile& tile)
{
    if (player.x > tile.x) // tile on the left
    {
        PLAYER_SET_LEFT(TILE_GET_RIGHT(tile));
    }
    else
    {
        PLAYER_SET_RIGHT(TILE_GET_LEFT(tile));
    }
}

void playerUpdate(TimeInterval dt)
{
    // input
    playerCrouching = !playerJumping && display.pressed(DOWN_BUTTON);
    
    bool jumpWasPressed = jumpPressed;
    jumpPressed = display.pressed(JUMP_BUTTON);
    if (!playerJumping && jumpPressed && !jumpWasPressed)
    {
        playerJumping = true;
        playerJumpSpeed = JUMP_SPEED;
        playerSetAnimation(PLAYER_ANIMATION_JUMP);
    }
    
    // update movement
    player.move = 0;
    
    if (!playerCrouching)
    {
        if (display.pressed(LEFT_BUTTON))
        {
            player.dir = DIR_LEFT;
            player.move = 1;
        }
        else if (display.pressed(RIGHT_BUTTON))
        {
            player.dir = DIR_RIGHT;
            player.move = 1;
        }
        
        if (player.move != 0 && display.pressed(RUN_BUTTON))
        {
            player.move = 2;
        }
    }
    
    playerJumpSpeed += GRAVITY;
    
    int16_t oldY = player.y;
    
    player.x += player.dir * player.move * WALK_SPEED;
    player.y += playerJumpSpeed;
    
    int16_t minX = PLAYER_LEFT;
    int16_t maxX = PLAYER_RIGHT;
    int16_t maxY = PLAYER_BOTTOM;
    
    Tile tile;
    
    if (GET_TILE(minX, player.y, tile) ||
        GET_TILE(maxX, player.y, tile))
    {
        playerHandleTileHorCollision(tile);
        minX = PLAYER_LEFT;
        maxX = PLAYER_RIGHT;
    }
    
    if (playerJumpSpeed > 0) // moving down
    {
        if (GET_TILE(minX, maxY, tile) ||
            GET_TILE(maxX, maxY, tile))
        {
            int16_t tileTop = TILE_GET_TOP(tile);
            int16_t oldBottom = oldY + PLAYER_HALF_HEIGHT;
            if (oldBottom <= tileTop) // player jumped on the tile
            {
                PLAYER_SET_BOTTOM(tileTop);
                playerJumpSpeed = 0;
                playerJumping = false;
            }
        }
    }
    else
    {
        int16_t minY = PLAYER_TOP;
        if (GET_TILE(minX, minY, tile) ||
            GET_TILE(maxX, minY, tile))
        {
            int16_t tileBottom = TILE_GET_BOTTOM(tile);
            int16_t oldTop = oldY - PLAYER_HALF_HEIGHT;
            if (oldTop >= tileBottom)
            {
                PLAYER_SET_TOP(tileBottom);
                playerJumpSpeed = 0;
            }
        }
    }
    
    // camera movement
    int16_t playerOffsetX = player.x - camX;
    int16_t playerOffsetY = player.y - camY;
    if (playerOffsetX > CAM_RANGE_X)
    {
        camX = player.x - CAM_RANGE_X;
    }
    else if (playerOffsetX < -CAM_RANGE_X)
    {
        camX = player.x + CAM_RANGE_X;
    }
    
    if (playerOffsetY > CAM_RANGE_Y)
    {
        camY = player.y - CAM_RANGE_Y;
    }
    else if (playerOffsetY < -CAM_RANGE_Y)
    {
        camY = player.y + CAM_RANGE_Y;
    }
    
    int16_t maxCamX = tileMapWidth - CAM_WIDTH_HALF;
    int16_t maxCamY = tileMapHeight - CAM_HEIGHT_HALF;
    camX = constrain(camX, CAM_WIDTH_HALF, maxCamX);
    camY = constrain(camY, CAM_HEIGHT_HALF, maxCamY);
    
    // update animation
    if (playerJumping)
    {
        playerSetAnimation(PLAYER_ANIMATION_JUMP);
    }
    else if (playerCrouching)
    {
        playerSetAnimation(PLAYER_ANIMATION_CROUCH);
    }
    else if (player.move == 0)
    {
        playerSetAnimation(PLAYER_ANIMATION_STAT);
    }
    else if (player.move == 1)
    {
        playerSetAnimation(PLAYER_ANIMATION_WALK);
    }
    else
    {
        playerSetAnimation(PLAYER_ANIMATION_RUN);
    }
    
    CharacterUpdateAnimation(&player, dt);
}

void playerDraw()
{
    DrawMode mode = DM_UNLIT;
    if (player.dir == DIR_LEFT)
    {
        mode |= DM_FLIP_X;
    }
    CharacterDraw(&player, mode);
}

void playerSetAnimation(int index)
{
    assert(index >= 0 && index < PLAYER_ANIMATIONS_COUNT);
    CharacterSetAnimation(&player, &PLAYER_ANIMATIONS[index]);
}

////////////////////////////////////////////////////////////////////
// Spider

void spiderUpdate(TimeInterval dt)
{
    
}

void spiderDraw()
{
    
}

////////////////////////////////////////////////////////////////////
// Tilemap

void tilemapDraw()
{
    TileMapDraw(&tileMap, 0, 0);
}

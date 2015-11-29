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

#include "tiles_tb_caverns.h"
#include "tiles_lair_01.h"

#define RUN_BUTTON  A_BUTTON
#define JUMP_BUTTON B_BUTTON

#define GRAVITY 5

#define JUMP_SPEED -65
#define WALK_SPEED 9
#define SLIDE_SPEED 12


static const uint8_t PLAYER_WIDTH                = S2W(8);
static const uint8_t PLAYER_HEIGHT               = S2W(8);
static const uint8_t PLAYER_COLLIDER_HALF_WIDTH  = S2W(3);
static const uint8_t PLAYER_COLLIDER_HALF_HEIGHT = S2W(4);

static const int16_t CAM_WIDTH         = S2W(128);
static const int16_t CAM_HEIGHT        = S2W(64);
static const int16_t CAM_WIDTH_HALF    = CAM_WIDTH / 2;
static const int16_t CAM_HEIGHT_HALF   = CAM_HEIGHT / 2;
static const int16_t CAM_RANGE_X       = S2W(30);
static const int16_t CAM_RANGE_Y       = S2W(15);

static Arduboy display;

static Character player = CharacterMake(PLAYER_WIDTH, PLAYER_HEIGHT);
static bool playerCrouching = false;
static bool playerJumping = false;
static bool jumpPressed = false;
static int16_t playerJumpSpeed = 0;
static Direction playerSlopeDir = 0;

static TileMap tileMap = TileMapMake(TILES_TB_CAVERNS, INDICES_LAIR_01, TILEMAP_LAIR_01_HEIGHT, TILEMAP_LAIR_01_WIDTH);

/* Tile map size in world space */
uint16_t tileMapWidth = TILEMAP_GET_WIDTH(tileMap);
uint16_t tileMapHeight = TILEMAP_GET_HEIGHT(tileMap);

/* Camera center pos in world space */
static int16_t camX;
static int16_t camY;

static TimeInterval lastFrameTime;
static TimeInterval lastDrawTime;

void playerUpdate(TimeInterval dt);
void playerDraw();
void playerSetAnimation(int index);

void tilemapDraw();

void startGame()
{
    display.start();
    drawInit(display.getBuffer());
    
    player.x = S2W(62);
    player.y = S2W(28);
    
    camX = CAM_WIDTH_HALF;
    camY = CAM_HEIGHT_HALF;
    lastFrameTime = millis();
}

void updateGame()
{
    TimeInterval dt = millis() - lastFrameTime;
    lastFrameTime = millis();
    
    playerUpdate(dt);
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
    
    display.display();
}

////////////////////////////////////////////////////////////////////
// Player

#define GET_TILE(X,Y,T) TileMapGetTile(&tileMap, X, Y, &T)
#define GET_TILE_SOLID(X,Y,T) (TileMapGetTile(&tileMap, X, Y, &T) > 15)

#define PLAYER_TOP           (player.y - PLAYER_COLLIDER_HALF_HEIGHT)
#define PLAYER_BOTTOM        (player.y + PLAYER_COLLIDER_HALF_HEIGHT)
#define PLAYER_LEFT          (player.x - (PLAYER_COLLIDER_HALF_WIDTH - 1))
#define PLAYER_RIGHT         (player.x + (PLAYER_COLLIDER_HALF_WIDTH - 1))

inline void PLAYER_SET_TOP(int16_t y)    { player.y = y + PLAYER_COLLIDER_HALF_HEIGHT; }
inline void PLAYER_SET_BOTTOM(int16_t y) { player.y = y - PLAYER_COLLIDER_HALF_HEIGHT; }
inline void PLAYER_SET_LEFT(int16_t x)   { player.x = x + PLAYER_COLLIDER_HALF_WIDTH; }
inline void PLAYER_SET_RIGHT(int16_t x)  { player.x = x - PLAYER_COLLIDER_HALF_WIDTH; }

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

void playerSlideSlope(const Tile& slopeTile, Direction dir)
{
    assert(slopeTile.index == TILE_SLOPE_RIGHT || slopeTile.index == TILE_SLOPE_LEFT);
    playerSlopeDir = dir;
    playerJumpSpeed = 0;
    playerJumping = false;
    
    int16_t offset = dir * (player.x - slopeTile.x);
    PLAYER_SET_BOTTOM(slopeTile.y + offset);
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
    
    int16_t oldY = player.y;
    
    playerJumpSpeed += GRAVITY;
    playerJumpSpeed = constrain(playerJumpSpeed, JUMP_SPEED, -JUMP_SPEED);
    
    if (playerSlopeDir == 0)
    {
        player.x += player.dir * player.move * WALK_SPEED;
        player.y += playerJumpSpeed;
    }
    else
    {
        if (playerJumpSpeed < 0)
        {
            player.y += playerJumpSpeed;
            playerSlopeDir = 0;
        }
        else
        {
            player.x += playerSlopeDir * SLIDE_SPEED;
        }
    }
    
    int16_t leftX   = PLAYER_LEFT;
    int16_t rightX  = PLAYER_RIGHT;
    int16_t bottomY = PLAYER_BOTTOM;

    if (playerJumpSpeed > 0) // moving down
    {
        Tile tile;
        uint8_t midIndex = GET_TILE(player.x, bottomY, tile);
        if (midIndex == TILE_SLOPE_LEFT)
        {
            playerSlideSlope(tile, DIR_LEFT);
        }
        else if (midIndex == TILE_SLOPE_RIGHT)
        {
            playerSlideSlope(tile, DIR_RIGHT);
        }
        else
        {
            if (playerSlopeDir == DIR_LEFT &&
                GET_TILE(player.x, bottomY + TILE_HEIGHT, tile) == TILE_SLOPE_LEFT)
            {
                playerSlideSlope(tile, DIR_LEFT);
            }
            else if (playerSlopeDir == DIR_RIGHT &&
                GET_TILE(player.x, bottomY + TILE_HEIGHT, tile) == TILE_SLOPE_RIGHT)
            {
                playerSlideSlope(tile, DIR_RIGHT);
            }
            else
            {
                playerSlopeDir = 0;
            }
        }
    }
    
    if (playerSlopeDir == 0)
    {
        Tile tile;

        if (GET_TILE(leftX, player.y, tile) ||
            GET_TILE(rightX, player.y, tile))
        {
            playerHandleTileHorCollision(tile);
            leftX = PLAYER_LEFT;
            rightX = PLAYER_RIGHT;
        }
        
        if (playerJumpSpeed > 0) // moving down
        {
            if (GET_TILE_SOLID(leftX, bottomY, tile) ||
                GET_TILE_SOLID(rightX, bottomY, tile))
            {
                int16_t tileTop = TILE_GET_TOP(tile);
                int16_t oldBottom = oldY + PLAYER_COLLIDER_HALF_HEIGHT;
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
            if (GET_TILE(player.x, minY, tile))
            {
                int16_t tileBottom = TILE_GET_BOTTOM(tile);
                int16_t oldTop = oldY - PLAYER_COLLIDER_HALF_HEIGHT;
                if (oldTop >= tileBottom)
                {
                    PLAYER_SET_TOP(tileBottom);
                    playerJumpSpeed = 0;
                }
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
    else if (playerJumpSpeed != 0 || playerSlopeDir != 0)
    {
        playerSetAnimation(PLAYER_ANIMATION_FALL);
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
// Tilemap

void tilemapDraw()
{
    TileMapDraw(&tileMap, 0, 0);
}

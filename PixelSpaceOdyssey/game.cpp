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

#define GRAVITY 50
#define FLOOR   416

#define JUMP_SPEED -66
#define WALK_SPEED 150

static Arduboy display;

static Character player = CharacterMake(8, 8);
static TileMap tileMap = TileMapMake(TILES_LAIR_01, INDICES_LAIR_01, TILEMAP_LAIR_01_WIDTH, TILEMAP_LAIR_01_HEIGHT);

static bool playerCrouching = false;
static bool playerJumping = false;
static int16_t playerJumpSpeed = 0;

static uint16_t camX, camY;

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
    
    player.x = SCREEN_TO_WORLD(8);
    player.y = SCREEN_TO_WORLD(32);
    
    camX = 0;
    camY = 0;
    
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

void playerUpdate(TimeInterval dt)
{
    // input
    // playerCrouching = display.pressed(DOWN_BUTTON);
    
    if (!playerJumping && display.pressed(JUMP_BUTTON))
    {
        playerJumping = true;
        playerJumpSpeed = JUMP_SPEED;
        playerSetAnimation(PLAYER_ANIMATION_JUMP);
    }
    
    if (!playerJumping)
    {
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
    }
    
    if (display.pressed(DOWN_BUTTON))
    {
        player.y += WALK_SPEED;
    }
    else if (display.pressed(UP_BUTTON))
    {
        player.y -= WALK_SPEED;
    }
    
    // update movement
    player.x += player.dir * player.move * WALK_SPEED;
//    if (x < MIN_X) x = MIN_X;
//    else if (x > MAX_X) x = MAX_X;
    
    if (playerJumping)
    {
        playerJumpSpeed += GRAVITY;
        player.y += playerJumpSpeed;
        
        if (player.y > FLOOR)
        {
            player.y = FLOOR;
            playerJumpSpeed = 0;
            playerJumping = false;
        }
    }
    
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
    TileMapDraw(&tileMap, -camX, -camY);
}

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

#include "tiles_merged.h"
#include "tiles_lair_01.h"
#include "enemies.h"
#include "hud.h"

#include "ch_hero_animations.h"

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

TileMap tileMap = TileMapMake(TILES_MERGED, INDICES_LAIR_01, TILEMAP_LAIR_01_HEIGHT, TILEMAP_LAIR_01_WIDTH);

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
    TileMapDraw(tileMap);
}

#pragma mark -
#pragma mark Player

#define PLAYER_TOP           (player.y - PLAYER_COLLIDER_HALF_HEIGHT)
#define PLAYER_BOTTOM        (player.y + PLAYER_COLLIDER_HALF_HEIGHT)
#define PLAYER_LEFT          (player.x - (PLAYER_COLLIDER_HALF_WIDTH - 1))
#define PLAYER_RIGHT         (player.x + (PLAYER_COLLIDER_HALF_WIDTH - 1))

#define RUN_BUTTON  A_BUTTON
#define SHOT_BUTTON B_BUTTON

const int16_t kPlayerAmmoMax = 7;
const int16_t kPlayerHealthMax = 10;

static const int8_t JUMP_SPEED  = -83;
static const int8_t SLIDE_SPEED = 15;
static const int8_t GRAVITY = 8;

static const uint8_t PLAYER_WIDTH                = S2W(8);
static const uint8_t PLAYER_HEIGHT               = S2W(8);
static const uint8_t PLAYER_COLLIDER_HALF_WIDTH  = S2W(3);
static const uint8_t PLAYER_COLLIDER_HALF_HEIGHT = S2W(4);

static const uint16_t PLAYER_DAMAGE_TIME = 3000;
static const uint16_t PLAYER_SHOCK_TIME = PLAYER_DAMAGE_TIME - 250;

inline void PLAYER_SET_TOP(int16_t y)    { player.y = y + PLAYER_COLLIDER_HALF_HEIGHT; }
inline void PLAYER_SET_BOTTOM(int16_t y) { player.y = y - PLAYER_COLLIDER_HALF_HEIGHT; }
inline void PLAYER_SET_LEFT(int16_t x)   { player.x = x + PLAYER_COLLIDER_HALF_WIDTH; }
inline void PLAYER_SET_RIGHT(int16_t x)  { player.x = x - PLAYER_COLLIDER_HALF_WIDTH; }

Character player;
uint8_t playerHealth;
uint8_t playerAmmo;

static bool playerCrouching = false;
static bool playerJumping = false;
static bool jumpPressed = false;
static int16_t playerJumpSpeed = 0;
static Direction playerSlopeDir = 0;
static int16_t playerDamageTime;
static bool playerDamageBlink;

static void playerSetAnimation(int index);

inline static bool isPlayerDamaged()
{
    return playerDamageTime > 0;
}

inline static bool isPlayerShocked()
{
    return playerDamageTime >= PLAYER_SHOCK_TIME;
}

inline static bool isPlayerDead()
{
    return playerHealth == 0;
}

void playerDie()
{
    assert(playerHealth == 0);
    playerCrouching = false;
    playerJumping = false;
    jumpPressed = false;
    playerSlopeDir = 0;
    playerDamageTime = 0;
    playerDamageBlink = false;
}

inline static void takeDamage(Direction dir)
{
    if (isPlayerDead())
    {
        return;
    }
    
    if (playerDamageTime <= 0)
    {
        playerDamageTime = PLAYER_DAMAGE_TIME;
        if (dir != DIR_NONE)
        {
            player.dir = dir;
            player.move = 1;
        }
        playerJumpSpeed = DIV2(JUMP_SPEED);
        
        if (playerHealth > 0)
        {
            --playerHealth;
            DISPATCH_DEBUG_EVENT(DEBUG_EVENT_PLAYER_DAMAGE);
            
            if (playerHealth == 0)
            {
                playerDie();
            }
        }
    }
}

inline static void pickupItem(int tileIndex)
{
    assert(!TileItemIsPicked(tileMap, tileIndex));
    
    int type = TileItemGetType(tileIndex);
    switch (type)
    {
        case ITEM_TYPE_HEALTH:
        {
            if (playerHealth < kPlayerHealthMax)
            {
                ++playerHealth;
                DISPATCH_DEBUG_EVENT(DEBUG_EVENT_PLAYER_DAMAGE); // TODO: fix event name
            }
            break;
        }
            
        case ITEM_TYPE_AMMO:
        {
            if (playerAmmo < kPlayerAmmoMax)
            {
                ++playerAmmo;
            }
            break;
        }
            
        case ITEM_TYPE_ROCK:
            break;
            
        default:
            assert(false); // TODO: output item type
            break;
    }
    
    TileItemSetPicked(tileMap, tileIndex, true);
}

inline static void playerHandleTileHorCollision(const Tile& tile)
{
    if (TILE_IS_HAZARD(tile.index))
    {
        takeDamage(-player.dir);
    }
    else if (TILE_IS_ITEM(tile.index))
    {
        pickupItem(tile.index);
    }
    else
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
}

inline static void playerSlideSlope(const Tile& slopeTile, Direction dir)
{
    assert(TILE_IS_SLOPE_LEFT(slopeTile.index) || TILE_IS_SLOPE_RIGHT(slopeTile.index));
    playerSlopeDir = dir;
    playerJumpSpeed = 0;
    playerJumping = false;
    
    int16_t offset = dir * (player.x - slopeTile.x);
    PLAYER_SET_BOTTOM(slopeTile.y + offset);
}

static inline void updateInput()
{
    // input
    playerCrouching = !playerJumping && buttonPressed(DOWN_BUTTON);
    
    bool jumpWasPressed = jumpPressed;
    jumpPressed = buttonPressed(UP_BUTTON);
    if (!playerJumping && jumpPressed && !jumpWasPressed)
    {
        playerJumping = true;
        playerJumpSpeed = JUMP_SPEED;
        playerSetAnimation(CH_HERO_ANIMATION_JUMP);
    }
    
    // update movement
    player.move = 0;
    
    if (!playerCrouching)
    {
        if (buttonPressed(LEFT_BUTTON))
        {
            player.dir = DIR_LEFT;
            player.move = 1;
        }
        else if (buttonPressed(RIGHT_BUTTON))
        {
            player.dir = DIR_RIGHT;
            player.move = 1;
        }
        
        if (player.move != 0 && buttonPressed(RUN_BUTTON))
        {
            player.move = 2;
        }
    }
}

void createPlayer()
{
    player = CharacterMake(PLAYER_WIDTH, PLAYER_HEIGHT);
    playerHealth = kPlayerHealthMax;
    playerAmmo = kPlayerAmmoMax;
    
    playerCrouching = false;
    playerJumping = false;
    jumpPressed = false;
    playerJumpSpeed = 0;
    playerSlopeDir = 0;
    playerDamageTime = 0;
    playerDamageBlink = false;
    
    playerSetAnimation(CH_HERO_ANIMATION_STAT);
}

inline static void playerUpdate(TimeInterval dt)
{
    // input
    if (!isPlayerShocked() && !isPlayerDead())
    {
        updateInput();
    }
    
    int16_t oldY = player.y;
    
    playerJumpSpeed += GRAVITY;
    playerJumpSpeed = constrain(playerJumpSpeed, JUMP_SPEED, -(JUMP_SPEED));
    
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
        uint8_t midIndex = getTile(player.x, bottomY, &tile);
        if (TILE_IS_SLOPE_LEFT(midIndex))
        {
            playerSlideSlope(tile, DIR_LEFT);
        }
        else if (TILE_IS_SLOPE_RIGHT(midIndex))
        {
            playerSlideSlope(tile, DIR_RIGHT);
        }
        else
        {
            if (playerSlopeDir == DIR_LEFT &&
                TILE_IS_SLOPE_LEFT(getTile(player.x, bottomY + TILE_HEIGHT, &tile)))
            {
                playerSlideSlope(tile, DIR_LEFT);
            }
            else if (playerSlopeDir == DIR_RIGHT &&
                     TILE_IS_SLOPE_RIGHT(getTile(player.x, bottomY + TILE_HEIGHT, &tile)))
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
        
        if (getTile(leftX, player.y, &tile) ||
            getTile(rightX, player.y, &tile))
        {
            playerHandleTileHorCollision(tile);
            leftX = PLAYER_LEFT;
            rightX = PLAYER_RIGHT;
        }
        
        if (playerJumpSpeed > 0) // moving down
        {
            if (getSolidTile(leftX, bottomY, &tile) ||
                getSolidTile(rightX, bottomY, &tile))
            {
                if (TILE_IS_HAZARD(tile.index))
                {
                    takeDamage(-player.dir);
                }
                else if (TILE_IS_ITEM(tile.index))
                {
                    pickupItem(tile.index);
                }
                else
                {
                    int16_t tileTop = TILE_GET_TOP(tile);
                    int16_t oldBottom = oldY + PLAYER_COLLIDER_HALF_HEIGHT;
                    if (oldBottom <= tileTop) // player jumped on the tile
                    {
                        PLAYER_SET_BOTTOM(tileTop);
                        playerJumpSpeed = 0;
                        playerJumping = false;
                        
                        if (isPlayerShocked() || isPlayerDead())
                        {
                            player.move = 0;
                        }
                    }
                }
            }
        }
        else
        {
            int16_t minY = PLAYER_TOP;
            if (getTile(player.x, minY, &tile))
            {
                if (TILE_IS_ITEM(tile.index))
                {
                    pickupItem(tile.index);
                }
                else
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
    
    // damage
    if (playerDamageTime > 0)
    {
        playerDamageTime -= dt;
        playerDamageBlink = !playerDamageBlink;
    }
    
    // update animation
    if (isPlayerShocked())
    {
        playerSetAnimation(CH_HERO_ANIMATION_IMPACT_BOTTOM);
    }
    else if (isPlayerDead())
    {
        playerSetAnimation(CH_HERO_ANIMATION_DEATH);
    }
    else if (playerJumping && playerJumpSpeed < 0)
    {
        playerSetAnimation(CH_HERO_ANIMATION_JUMP);
    }
    else if (playerJumpSpeed != 0 || playerSlopeDir != 0)
    {
        playerSetAnimation(CH_HERO_ANIMATION_FALL);
    }
    else if (playerCrouching)
    {
        playerSetAnimation(CH_HERO_ANIMATION_CROUNCH);
    }
    else if (player.move == 0)
    {
        playerSetAnimation(CH_HERO_ANIMATION_STAT);
    }
    else if (player.move == 1)
    {
        playerSetAnimation(CH_HERO_ANIMATION_WALK);
    }
    else
    {
        playerSetAnimation(CH_HERO_ANIMATION_RUN);
    }
    
    // update last seen position
    if (!playerJumping)
    {
        playerLastSeenX = player.x;
        playerLastSeenY = player.y;
    }
    
    CharacterUpdate(&player, dt);
}

void playerDraw()
{
    if (isPlayerDamaged() && playerDamageBlink)
    {
        return;
    }
    CharacterDraw(&player);
}

void playerDamage(Character *enemy)
{
    takeDamage(enemy->x > player.x ? DIR_LEFT : DIR_RIGHT);
}

void playerSetAnimation(int index)
{
    assert(index >= 0 && index < CH_HERO_ANIMATIONS_COUNT);
    CharacterSetAnimation(&player, &CH_HERO_ANIMATIONS[index]);
}

#pragma mark -
#pragma mark Game

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
    enemies[enemiesCount].targetPos = x;
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

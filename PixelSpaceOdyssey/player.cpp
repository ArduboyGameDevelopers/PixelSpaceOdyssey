#include "player.h"
#include "game.h"
#include "Arduboy.h"
#include "player_animations.h"

#define PLAYER_TOP           (player.y - PLAYER_COLLIDER_HALF_HEIGHT)
#define PLAYER_BOTTOM        (player.y + PLAYER_COLLIDER_HALF_HEIGHT)
#define PLAYER_LEFT          (player.x - (PLAYER_COLLIDER_HALF_WIDTH - 1))
#define PLAYER_RIGHT         (player.x + (PLAYER_COLLIDER_HALF_WIDTH - 1))

#define RUN_BUTTON  A_BUTTON
#define SHOT_BUTTON B_BUTTON

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

inline static void takeDamage(Direction dir)
{
    if (playerDamageTime <= 0)
    {
        playerDamageTime = PLAYER_DAMAGE_TIME;
        if (dir != DIR_NONE)
        {
            player.dir = dir;
            player.move = 1;
        }
        playerJumpSpeed = DIV2(JUMP_SPEED);
    }
}

inline static void playerHandleTileHorCollision(const Tile& tile)
{
    if (TILE_IS_HAZARD(tile.index))
    {
        takeDamage(-player.dir);
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
    assert(slopeTile.index == TILE_SLOPE_RIGHT || slopeTile.index == TILE_SLOPE_LEFT);
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
        playerSetAnimation(PLAYER_ANIMATION_JUMP);
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
    
    playerCrouching = false;
    playerJumping = false;
    jumpPressed = false;
    playerJumpSpeed = 0;
    playerSlopeDir = 0;
    playerDamageTime = 0;
    playerDamageBlink = false;
}

void playerUpdate(TimeInterval dt)
{
    // input
    if (!isPlayerShocked())
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
                getTile(player.x, bottomY + TILE_HEIGHT, &tile) == TILE_SLOPE_LEFT)
            {
                playerSlideSlope(tile, DIR_LEFT);
            }
            else if (playerSlopeDir == DIR_RIGHT &&
                getTile(player.x, bottomY + TILE_HEIGHT, &tile) == TILE_SLOPE_RIGHT)
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
                else
                {
                    int16_t tileTop = TILE_GET_TOP(tile);
                    int16_t oldBottom = oldY + PLAYER_COLLIDER_HALF_HEIGHT;
                    if (oldBottom <= tileTop) // player jumped on the tile
                    {
                        PLAYER_SET_BOTTOM(tileTop);
                        playerJumpSpeed = 0;
                        playerJumping = false;
                        
                        if (isPlayerShocked())
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

    // damage
    if (playerDamageTime > 0)
    {
        playerDamageTime -= dt;
        playerDamageBlink = !playerDamageBlink;
    }

    // update animation
    if (isPlayerShocked())
    {
        playerSetAnimation(PLAYER_ANIMATION_IMPACT_BOTTOM);
    }
    else if (playerJumping)
    {
        playerSetAnimation(PLAYER_ANIMATION_JUMP);
    }
    else if (playerJumpSpeed != 0 || playerSlopeDir != 0)
    {
        playerSetAnimation(PLAYER_ANIMATION_FALL);
    }
    else if (playerCrouching)
    {
        playerSetAnimation(PLAYER_ANIMATION_CROUNCH);
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
    assert(index >= 0 && index < PLAYER_ANIMATIONS_COUNT);
    CharacterSetAnimation(&player, &PLAYER_ANIMATIONS[index]);
}

#include "enemy_base.h"

#include "game.h"

inline static void handleObstacle(Character *character)
{
    CharacterCallbackInvoke(character, CHARACTER_CALLBACK_OBSTACLE);
}

void EnemyUpdate(Character* self, TimeInterval dt)
{
    // update AI
    CharacterBehaviour behaviour = self->behaviour;
    if (behaviour)
    {
        behaviour(self, dt);
    }
    
    // update movement
    int8_t move = self->move;
    if (move != 0)
    {
        self->x += self->dir * self->move * WALK_SPEED;
        
        Direction dir = self->dir;
        if (dir == DIR_LEFT && !EnemyCanMoveLeft(self))
        {
            CharacterSetLeft(self, self->moveMinX);
            handleObstacle(self);
        }
        else if (dir == DIR_RIGHT && !EnemyCanMoveRight(self))
        {
            CharacterSetRight(self, self->moveMaxX);
            handleObstacle(self);
        }
    }
    
    // update character
    CharacterUpdate(self, dt);
}

#pragma mark -
#pragma mark Constraints

void UpdateConstraints(Character *character)
{
    // handle collisions (only horizontal)
    Tile tile;

    int16_t top = CharacterGetTop(character);

    int col = CharacterGetCol(character);

    int16_t moveMinX = 0;
    int16_t moveMaxX = TILEMAP_GET_WIDTH(tileMap);
    int16_t sightMinX = moveMinX;
    int16_t sightMaxX = moveMaxX;
    
    bool checkMoveMinX = true;
    bool checkMoveMaxX = true;
    bool checkSightMinX = true;
    bool checkSightMaxX = true;
    
    for (int c = col - 1; c >= 0 && (checkSightMinX || checkMoveMinX); --c)
    {
        int16_t tx = c * TILE_WIDTH + DIV2(TILE_WIDTH);
        if (getSolidTile(tx, character->y, &tile))
        {
            if (checkSightMinX)
            {
                sightMinX = TILE_GET_RIGHT(tile);
                checkSightMinX = false;
            }
            if (checkMoveMinX)
            {
                moveMinX = TILE_GET_RIGHT(tile);
                checkMoveMinX = false;
            }
        }
        
        if (checkMoveMinX && getSolidTile(tx, top, &tile))
        {
            moveMinX = TILE_GET_RIGHT(tile);
            checkMoveMinX = false;
        }
        
        if (checkMoveMinX && !getSolidTile(tx, character->y + TILE_HEIGHT, &tile))
        {
            moveMinX = tx + DIV2(TILE_WIDTH);
            checkMoveMinX = false;
        }
    }
    
    for (int c = col + 1; c < tileMap.cols && (checkSightMaxX || checkMoveMaxX); ++c)
    {
        int16_t tx = c * TILE_WIDTH + DIV2(TILE_WIDTH);
        if (getSolidTile(tx, character->y, &tile))
        {
            if (checkSightMaxX)
            {
                sightMaxX = TILE_GET_LEFT(tile);
                checkSightMaxX = false;
            }
            if (checkMoveMaxX)
            {
                moveMaxX = TILE_GET_LEFT(tile);
                checkMoveMaxX = false;
            }
        }
        
        if (checkMoveMaxX && getSolidTile(tx, top, &tile))
        {
            moveMaxX = TILE_GET_LEFT(tile);
            checkMoveMaxX = false;
        }
        
        if (checkMoveMaxX && !getSolidTile(tx, character->y + TILE_HEIGHT, &tile))
        {
            moveMaxX = tx - DIV2(TILE_WIDTH);
            checkMoveMaxX = false;
        }
    }
    
    character->moveMinX = moveMinX;
    character->moveMaxX = moveMaxX;
    character->sightMinX = sightMinX;
    character->sightMaxX = sightMaxX;
}

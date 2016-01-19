#include "enemy_base.h"

#include "game.h"

inline static void EnemyReachedObstacle(Character *character)
{
    CharacterCallbackInvoke(character, CHARACTER_CALLBACK_OBSTACLE);
}

inline static void EnemyReachedTarget(Character *self)
{
    CharacterCallbackInvoke(self, CHARACTER_CALLBACK_REACHED_TARGET);
}

void EnemyUpdate(Character* self, TimeInterval dt)
{
    // update AI
    CharacterBehaviour behaviour = self->behaviour;
    if (behaviour)
    {
        behaviour(self, dt);
    }
    
    // move to target
    int16_t distance = self->targetPos - self->x;
    if (distance != 0 && CharacterIsMoving(self))
    {
        int16_t offset = self->move * WALK_SPEED; // TODO: use time interval
        self->dir = distance > 0 ? DIR_RIGHT : DIR_LEFT;
        
        if (offset < abs(distance))
        {
            self->x += self->dir * offset;
        }
        else
        {
            self->x += distance;
        }
        
        if (self->dir == DIR_LEFT && self->x < self->moveMinX)
        {
            self->x = self->moveMinX;
            EnemyReachedObstacle(self);
        }
        else if (self->dir == DIR_RIGHT && self->x > self->moveMaxX)
        {
            self->x = self->moveMaxX;
            EnemyReachedObstacle(self);
        }
        else if (self->x == self->targetPos)
        {
            EnemyReachedTarget(self);
        }
    }
    
//    // update movement
//    int8_t move = self->move;
//    if (move != 0)
//    {
//        self->x += self->dir * self->move * WALK_SPEED;
//        
//        Direction dir = self->dir;
//        if (dir == DIR_LEFT && !EnemyCanMoveLeft(self))
//        {
//            CharacterSetLeft(self, self->moveMinX);
//            handleObstacle(self);
//        }
//        else if (dir == DIR_RIGHT && !EnemyCanMoveRight(self))
//        {
//            CharacterSetRight(self, self->moveMaxX);
//            handleObstacle(self);
//        }
//    }
    
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
    
    character->moveMinX = moveMinX + DIV2(character->width);
    character->moveMaxX = moveMaxX - DIV2(character->width);
    character->sightMinX = sightMinX;
    character->sightMaxX = sightMaxX;
}

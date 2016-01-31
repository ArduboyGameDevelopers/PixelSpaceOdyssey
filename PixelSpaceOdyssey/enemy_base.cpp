#include "enemy_base.h"

#include "game.h"

inline static void EnemyReachedObstacle(Character *self)
{
    self->move = 0;             // stop moving
    self->hasTarget = false;    // reach the target
    CharacterCallbackInvoke(self, CHARACTER_CALLBACK_OBSTACLE);
}

inline static void EnemyReachedTarget(Character *self)
{
    self->move = 0;             // stop moving
    self->hasTarget = false;    // reach the target
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
    if (self->hasTarget)
    {
        int16_t distance = self->targetPos - self->x;
        if (CharacterIsMoving(self))
        {
            int16_t offset = self->move * WALK_SPEED;       // FIXME: use time interval
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
                self->x = self->moveMinX;   // fix the position
                EnemyReachedObstacle(self);
            }
            else if (self->dir == DIR_RIGHT && self->x > self->moveMaxX)
            {
                self->x = self->moveMaxX;   // fix the position
                EnemyReachedObstacle(self);
            }
            else if (self->x == self->targetPos)
            {
                EnemyReachedTarget(self);
            }
        }
    }

    // update character
    CharacterUpdate(self, dt);
}

#pragma mark -
#pragma mark States

void EnemyAttackBaseBase(Character *self)
{
    self->lastAttackTimestamp = millis();
    playerDamage(self);
}

void EnemySetTargetPos(Character *self, int16_t target)
{
    self->targetPos = target;
    self->hasTarget = true;
}

void EnemySetState(Character *self, EnemyState state)
{
    self->state = (uint16_t) state;
    self->stateTime = 0;
    self->move = 0;

    int8_t index = self->animationLookup[state];
    CharacterSetAnimation(self, &self->animations[index]);
}

void EnemyChase(Character *self)
{
    DEBUG_LOG("Enemy: chase");

    EnemySetState(self, EnemyStateChase);
    self->move = self->moveMax;
}

void EnemyStat(Character *self, bool canPatrol)
{
    DEBUG_LOG("Enemy: stat");

    EnemySetState(self, EnemyStateStat);
    self->stateTime = canPatrol ? random(1500, 3000) : 0;
}

void EnemyAttack(Character *self)
{
    DEBUG_LOG("Enemy: attack");

    self->lastAttackTimestamp = millis();
    playerDamage(self);

    EnemySetState(self, EnemyStateAttack);
}

void EnemyPatrol(Character *self)
{
    switch (self->patrollingType)
    {
        case CharacterPatrollingTypeNone: // don't patrol: stay your ground
        {
            DEBUG_LOG("Enemy: don't patrol. Stay your ground");

            EnemyStat(self);
            break;
        }

        case CharacterPatrollingTypeReturnToBase:
        {
            if (self->targetPos != self->basePos)
            {
                DEBUG_LOG("Enemy: return to base");

                EnemySetState(self, EnemyStatePatrol);
                EnemySetTargetPos(self, self->basePos); // return to the spawn point
                self->move = 3;
            }
            else
            {
                DEBUG_LOG("Enemy: already at the base");
            }
            break;
        }

        default:
        {
            int16_t targetPos = self->targetPos;

            if (self->dir == DIR_LEFT)
            {
                if (EnemyCanMoveLeft(self))
                {
                    targetPos = self->moveMinX;
                }
                else if (EnemyCanMoveRight(self))
                {
                    self->dir = DIR_RIGHT;
                    targetPos = self->moveMaxX;
                }
            }
            else if (self->dir == DIR_RIGHT)
            {
                if (EnemyCanMoveRight(self))
                {
                    targetPos = self->moveMaxX;
                }
                else if (EnemyCanMoveLeft(self))
                {
                    self->dir = DIR_LEFT;
                    targetPos = self->moveMinX;
                }
            }

            if (self->targetPos != targetPos)
            {
                DEBUG_LOG("Enemy: patrol");

                EnemySetState(self, EnemyStatePatrol);
                self->move = self->moveMax;
                EnemySetTargetPos(self, targetPos);
            }
            else
            {
                DEBUG_LOG("Enemy: can't patrol");
            }
            break;
        }
    }
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

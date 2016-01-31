#include "dog.h"

#include "animation.h"
#include "ch_dog_animations.h"

#include "game.h"

static inline void setAnimation(Character *self, int index)
{
    CharacterSetAnimation(self, &self->animations[index]);
}

static inline void setState(Character *self, EnemyState state)
{
    self->state = (uint16_t) state;
    self->stateTime = 0;
    self->move = 0;

    setAnimation(self, self->animationLookup[state]);
}

static inline void chase(Character *self)
{
    DEBUG_LOG("Dog: chase");

    setState(self, EnemyStateChase);
    self->move = self->moveMax;
}

static inline void stat(Character *self, bool canPatrol = true)
{
    DEBUG_LOG("Dog: stat");

    setState(self, EnemyStateStat);
    self->stateTime = canPatrol ? random(1500, 3000) : 0;
}

static inline void attack(Character *self)
{
    DEBUG_LOG("Dog: attack");

    EnemyAttack(self);
    setState(self, EnemyStateAttack);
}

static inline void patrol(Character *self)
{
    switch (self->patrollingType)
    {
        case CharacterPatrollingTypeNone: // don't patrol: stay your ground
        {
            DEBUG_LOG("Dog: don't patrol. Stay your ground");

            stat(self);
            break;
        }
            
        case CharacterPatrollingTypeReturnToBase:
        {
            if (self->targetPos != self->basePos)
            {
                DEBUG_LOG("Dog: return to base");

                setState(self, EnemyStatePatrol);
                EnemySetTargetPos(self, self->basePos); // return to the spawn point
                self->move = 3;
            }
            else
            {
                DEBUG_LOG("Dog: already at the base");
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
                DEBUG_LOG("Dog: patrol");

                setState(self, EnemyStatePatrol);
                self->move = self->moveMax;
                EnemySetTargetPos(self, targetPos);
            }
            else
            {
                DEBUG_LOG("Dog: can't patrol");
            }
            break;
        }
    }
}

void EnemyInitDog(Character *self)
{
    if (self->initialState == CharacterInitialStatePatrol)
    {
        patrol(self);
    }
    else
    {
        stat(self, false);
    }
}

void EnemyCallbackDog(Character *self, CharacterCallbackType type, int16_t, int16_t)
{
    switch (type)
    {
        case CHARACTER_CALLBACK_ANIMATION_FINISHED:
        {
            EnemyState state = EnemyGetState(self);
            switch (state)
            {
                case EnemyStateAttack:
                    stat(self);
                    break;
            }
            break;
        }
            
        case CHARACTER_CALLBACK_REACHED_TARGET:
        {
            EnemyState state = EnemyGetState(self);
            switch (state)
            {
                case EnemyStateChase:
                {
                    if (self->canSeePlayer &&
                        EnemyIsCloseToAttack(self) &&
                        EnemyCanAttack(self))
                    {
                        attack(self);
                    }
                    else
                    {
                        stat(self);
                    }
                    break;
                }
                    
                case EnemyStatePatrol:
                {
                    stat(self);
                    break;
                }
            }
            break;
        }
        
        case CHARACTER_CALLBACK_OBSTACLE:
        {
            EnemyState state = EnemyGetState(self);
            switch (state)
            {
                case EnemyStateChase:
                    stat(self);
                    break;
            }
            break;
        }
    }
}

void EnemyBehaviourDog(Character *self, TimeInterval dt)
{
    EnemyState state = EnemyGetState(self);
    switch (state)
    {
        case EnemyStateStat:
        case EnemyStatePatrol:
        case EnemyStateChase:
        {
            // update last seen pos
            EnemyUpdatePlayerPos(self);

            // see the player?
            if (self->canSeePlayer)
            {
                int16_t distance = self->lastPlayerX - self->x;  // distance between enemy and last player's seen pos
                self->dir = distance < 0 ? DIR_LEFT : DIR_RIGHT; // face the player

                if (EnemyIsCloseToAttack(self))
                {
                    if (EnemyCanAttack(self))
                    {
                        attack(self);
                    }
                    else if (CharacterIsMoving(self))
                    {
                        stat(self);
                    }
                }
                else if (distance < 0 && EnemyCanMoveLeft(self) || // to far away to attack
                         distance > 0 && EnemyCanMoveRight(self))  // but can move towards the player
                {
                    // set a new target
                    int16_t minDistance = DIV2(self->width + player.width); // how close enemy gets to a player
                    EnemySetTargetPos(self, distance > 0 ? self->lastPlayerX - minDistance : self->lastPlayerX + minDistance);

                    // if not chasing already - start chasing
                    if (state != EnemyStateChase)
                    {
                        chase(self);
                    }
                }
            }
            else if (state == EnemyStateStat) // stay there and can't see the player - wait until enemy can patrol again
            {
                if (self->stateTime > dt)
                {
                    self->stateTime -= dt;
                }
                else if (self->stateTime > 0)
                {
                    self->stateTime = 0;
                    patrol(self);
                }
            }
            break;
        }
    }
}



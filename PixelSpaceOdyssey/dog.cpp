#include "dog.h"

#include "animation.h"
#include "ch_dog_animations.h"

#include "game.h"

#define DogStateStat    0
#define DogStateChase   1
#define DogStateAttack  2
#define DogStatePatrol  3

static const uint8_t ANIMATION_LOOKUP[] = {
    CH_DOG_ANIMATION_STAT,     /* DogStateStat */
    CH_DOG_ANIMATION_RUN,      /* DogStateRun */
    CH_DOG_ANIMATION_ATTACK,   /* DogStateAttack */
    CH_DOG_ANIMATION_RUN       /* DogStatePatrol */
};

typedef uint16_t DogState;

static inline void setAnimation(Character *self, int index)
{
    assert(index >= 0 && index < CH_DOG_ANIMATIONS_COUNT);
    CharacterSetAnimation(self, &CH_DOG_ANIMATIONS[index]);
}

static inline DogState getState(Character *self)
{
    return (DogState) self->state;
}

static inline void setState(Character *self, DogState state)
{
    self->state = (uint16_t) state;
    self->stateTime = 0;
    self->move = 0;
    
    setAnimation(self, ANIMATION_LOOKUP[state]);
}

static inline void chase(Character *self)
{
    DEBUG_LOG("Dog: chase");

    setState(self, DogStateChase);
    self->move = 3;
}

static inline void stat(Character *self, bool canPatrol = true)
{
    DEBUG_LOG("Dog: stat");

    setState(self, DogStateStat);
    self->stateTime = canPatrol ? random(1500, 3000) : 0;
}

static inline void attack(Character *self)
{
    DEBUG_LOG("Dog: attack");

    EnemyAttack(self);
    setState(self, DogStateAttack);
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

                setState(self, DogStatePatrol);
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

                setState(self, DogStatePatrol);
                self->move = 3;
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
            DogState state = getState(self);
            switch (state)
            {
                case DogStateAttack:
                    stat(self);
                    break;
            }
            break;
        }
            
        case CHARACTER_CALLBACK_REACHED_TARGET:
        {
            DogState state = getState(self);
            switch (state)
            {
                case DogStateChase:
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
                    
                case DogStatePatrol:
                {
                    stat(self);
                    break;
                }
            }
            break;
        }
        
        case CHARACTER_CALLBACK_OBSTACLE:
        {
            DogState state = getState(self);
            switch (state)
            {
                case DogStateChase:
                    stat(self);
                    break;
            }
            break;
        }
    }
}

void EnemyBehaviourDog(Character *self, TimeInterval dt)
{
    DogState state = getState(self);
    switch (state)
    {
        case DogStateStat:
        case DogStatePatrol:
        case DogStateChase:
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
                    if (state != DogStateChase)
                    {
                        chase(self);
                    }
                }
            }
            else if (state == DogStateStat) // stay there and can't see the player - wait until enemy can patrol again
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



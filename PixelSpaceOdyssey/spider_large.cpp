#include "spider_large.h"

#include "animation.h"
#include "ch_spider_large_animations.h"

#include "game.h"

static const EnemyState EnemyStateSleep   = EnemyStateUser;
static const EnemyState EnemyStateAwaken  = EnemyStateUser + 1;
static const EnemyState EnemyStateRise    = EnemyStateUser + 2;

static inline void setAnimation(Character *self, int index)
{
    assert(index >= 0 && index < CH_SPIDER_LARGE_ANIMATIONS_COUNT);
    CharacterSetAnimation(self, &CH_SPIDER_LARGE_ANIMATIONS[index]);
}

static inline EnemyState getState(Character *self)
{
    return (EnemyState) self->state;
}

static inline void setState(Character *self, EnemyState state)
{
    self->state = (uint16_t) state;
    self->stateTime = 0;
    self->move = 0;
    setAnimation(self, CH_SPIDER_LARGE_ANIMATION_LOOKUP[state]);
}

static inline void awake(Character *self)
{
    setState(self, EnemyStateAwaken);
}

static inline void rise(Character *self)
{
    setState(self, EnemyStateRise);
}

static inline void chase(Character *self)
{
    setState(self, EnemyStateChase);
    self->move = 1;
}

static inline void patrol(Character *self)
{
    setState(self, EnemyStatePatrol);
    self->move = 1;
}

static inline void stat(Character *self)
{
    setState(self, EnemyStateStat);
}

static inline void attack(Character *self)
{
    setState(self, EnemyStateAttack);
    EnemyAttackBase(self);
}

void EnemyInitSpiderLarge(Character *character)
{
    setState(character, EnemyStateSleep);
}

void EnemyCallbackSpiderLarge(Character *self, CharacterCallbackType type, int16_t, int16_t)
{
    EnemyState state = getState(self);
    switch (type)
    {
        case CHARACTER_CALLBACK_ANIMATION_FINISHED:
        {
            switch (state)
            {
                case EnemyStateRise:
                    chase(self);
                    break;
                    
                case EnemyStateAttack:
                    chase(self);
                    break;
            }
            break;
        }
            
        case CHARACTER_CALLBACK_OBSTACLE:
        {
            switch (state)
            {
                case EnemyStateChase:
                    stat(self);
                    break;
                case EnemyStatePatrol:
                    self->dir = -self->dir;
                    break;
            }
            break;
        }
    }
}

void EnemyBehaviourSpiderLarge(Character *self, TimeInterval dt)
{
    EnemyState state = getState(self);
    
    switch (state)
    {
        case EnemyStateSleep:
        {
            EnemyUpdatePlayerPos(self);
            if (self->canSeePlayer)
            {
                awake(self);
            }
            break;
        }
        case EnemyStateAwaken:
        {
            self->stateTime += dt;
            if (self->stateTime > 500)
            {
                rise(self);
            }
            break;
        }
        case EnemyStateRise:
        {
            break;
        }
        case EnemyStateChase:
        case EnemyStateStat:
        {
            EnemyUpdatePlayerPos(self);
            
            int16_t distance = self->lastPlayerX - self->x;
            self->dir = distance < 0 ? DIR_LEFT : DIR_RIGHT;
            
            if (abs(distance) < (self->canSeePlayer ? DIV2(self->width + player.width) : 10))
            {
                if (self->canSeePlayer && EnemyCanAttack(self))
                {
                    attack(self);
                }
                else
                {
                    stat(self);
                }
            }
            else if (self->canSeePlayer)
            {
                self->stateTime = 0;
                if (self->move == 0)
                {
                    chase(self);
                }
            }
            else if (self->move == 0)
            {
                self->stateTime += dt;
                if (self->stateTime > 5000)
                {
                    patrol(self);
                }
            }
            break;
        }
            
        case EnemyStatePatrol:
        {
            EnemyUpdatePlayerPos(self);
            if (self->canSeePlayer)
            {
                chase(self);
            }
            break;
        }
            
    }
}

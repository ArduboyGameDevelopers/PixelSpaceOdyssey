#include "spider_large.h"

#include "animation.h"
#include "ch_spider_large_animations.h"

#include "game.h"

#define SpiderLargeStateSleep   0
#define SpiderLargeStateAwaken  1
#define SpiderLargeStateRise    2
#define SpiderLargeStateWalk    3
#define SpiderLargeStateAttack  4
#define SpiderLargeStateStat    5
#define SpiderLargeStatePatrol  6

static const uint8_t ANIMATION_LOOKUP[] = {
    CH_SPIDER_LARGE_ANIMATION_SLEEP,   /* SpiderLargeStateSleep */
    CH_SPIDER_LARGE_ANIMATION_AWAKEN,  /* SpiderLargeStateAwaken */
    CH_SPIDER_LARGE_ANIMATION_RISE,    /* SpiderLargeStateRise */
    CH_SPIDER_LARGE_ANIMATION_WALK,    /* SpiderLargeStateWalk */
    CH_SPIDER_LARGE_ANIMATION_ATTACK,  /* SpiderLargeStateAttack */
    CH_SPIDER_LARGE_ANIMATION_STAT,    /* SpiderLargeStateStat */
    CH_SPIDER_LARGE_ANIMATION_WALK,    /* SpiderLargeStatePatrol */
};

typedef uint16_t SpiderLargeState;

static inline void setAnimation(Character *self, int index)
{
    assert(index >= 0 && index < CH_SPIDER_LARGE_ANIMATIONS_COUNT);
    CharacterSetAnimation(self, &CH_SPIDER_LARGE_ANIMATIONS[index]);
}

static inline SpiderLargeState getState(Character *self)
{
    return (SpiderLargeState) self->state;
}

static inline void setState(Character *self, SpiderLargeState state)
{
    self->state = (uint16_t) state;
    self->stateTime = 0;
    self->move = 0;
    setAnimation(self, ANIMATION_LOOKUP[state]);
}

static inline void awake(Character *self)
{
    setState(self, SpiderLargeStateAwaken);
}

static inline void rise(Character *self)
{
    setState(self, SpiderLargeStateRise);
}

static inline void walk(Character *self)
{
    setState(self, SpiderLargeStateWalk);
    self->move = 1;
}

static inline void patrol(Character *self)
{
    setState(self, SpiderLargeStatePatrol);
    self->move = 1;
}

static inline void stat(Character *self)
{
    setState(self, SpiderLargeStateStat);
}

static inline void attack(Character *self)
{
    setState(self, SpiderLargeStateAttack);
    EnemyAttack(self);
}

void EnemyInitSpiderLarge(Character *character)
{
    setState(character, SpiderLargeStateSleep);
}

void EnemyCallbackSpiderLarge(Character *self, CharacterCallbackType type, int16_t, int16_t)
{
    SpiderLargeState state = getState(self);
    switch (type)
    {
        case CHARACTER_CALLBACK_ANIMATION_FINISHED:
        {
            switch (state)
            {
                case SpiderLargeStateRise:
                    walk(self);
                    break;
                    
                case SpiderLargeStateAttack:
                    walk(self);
                    break;
            }
            break;
        }
            
        case CHARACTER_CALLBACK_OBSTACLE:
        {
            switch (state)
            {
                case SpiderLargeStateWalk:
                    stat(self);
                    break;
                case SpiderLargeStatePatrol:
                    self->dir = -self->dir;
                    break;
            }
            break;
        }
    }
}

void EnemyBehaviourSpiderLarge(Character *self, TimeInterval dt)
{
    SpiderLargeState state = getState(self);
    
    switch (state)
    {
        case SpiderLargeStateSleep:
        {
            EnemyUpdatePlayerPos(self);
            if (self->canSeePlayer)
            {
                awake(self);
            }
            break;
        }
        case SpiderLargeStateAwaken:
        {
            self->stateTime += dt;
            if (self->stateTime > 500)
            {
                rise(self);
            }
            break;
        }
        case SpiderLargeStateRise:
        {
            break;
        }
        case SpiderLargeStateWalk:
        case SpiderLargeStateStat:
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
                    walk(self);
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
            
        case SpiderLargeStatePatrol:
        {
            EnemyUpdatePlayerPos(self);
            if (self->canSeePlayer)
            {
                walk(self);
            }
            break;
        }
            
    }
}

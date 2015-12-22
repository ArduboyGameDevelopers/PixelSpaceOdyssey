#include "spider_large.h"

#include "animation.h"
#include "spider_large_animations.h"

#include "game.h"

#define SpiderLargeStateSleep   0
#define SpiderLargeStateAwaken  1
#define SpiderLargeStateRise    2
#define SpiderLargeStateWalk    3
#define SpiderLargeStateAttack  4
#define SpiderLargeStateStat    5

#define SPIDER_LARGE_USER_MASK_ATTACK 0x01

static const uint8_t ANIMATION_LOOKUP[] = {
    SPIDER_LARGE_ANIMATION_SLEEP,   /* SpiderLargeStateSleep */
    SPIDER_LARGE_ANIMATION_AWAKEN,  /* SpiderLargeStateAwaken */
    SPIDER_LARGE_ANIMATION_RISE,    /* SpiderLargeStateRise */
    SPIDER_LARGE_ANIMATION_WALK,    /* SpiderLargeStateWalk */
    SPIDER_LARGE_ANIMATION_ATTACK,  /* SpiderLargeStateAttack */
    SPIDER_LARGE_ANIMATION_STAT,    /* SpiderLargeStateStat */
};

typedef uint16_t SpiderLargeState;

static void attackEnableCallback(void *user, int16_t data1, int16_t data2);

static inline void setAnimation(Character *self, int index)
{
    assert(index >= 0 && index < SPIDER_LARGE_ANIMATIONS_COUNT);
    CharacterSetAnimation(self, &SPIDER_LARGE_ANIMATIONS[index]);
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

static inline bool canAttack(const Character *self)
{
    return CharacterHasUserFlag(self, SPIDER_LARGE_USER_MASK_ATTACK);
}

static inline void setCanAttack(Character *self, bool flag)
{
    CharacterSetUserFlag(self, SPIDER_LARGE_USER_MASK_ATTACK, flag);
}

static inline void awake(Character *self)
{
    setState(self, SpiderLargeStateAwaken);
}

static inline void rise(Character *self)
{
    setState(self, SpiderLargeStateRise);
    setCanAttack(self, true);
}

static inline void walk(Character *self)
{
    setState(self, SpiderLargeStateWalk);
    self->move = 1;
}

static inline void stat(Character *self)
{
    setState(self, SpiderLargeStateStat);
}

static inline void attack(Character *self)
{
    setState(self, SpiderLargeStateAttack);
    setCanAttack(self, false);
    DispatchSchedule(attackEnableCallback, 500, self);
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
            if (state == SpiderLargeStateWalk)
            {
                stat(self);
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
            
            if (abs(distance) < (self->canSeePlayer ? DIV2(self->colliderWidth + player.width) : 10))
            {
                if (self->canSeePlayer && canAttack(self))
                {
                    attack(self);
                }
                else
                {
                    stat(self);
                }
            }
            else if (self->move == 0)
            {
                walk(self);
            }
            break;
        }
        
        case SpiderLargeStateAttack:
        {
            break;
        }
    }
}

#pragma mark -
#pragma mark Dispatcher callbacks

static void attackEnableCallback(void *user, int16_t, int16_t)
{
    Character *self = (Character *)user;
    setCanAttack(self, true);
}


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

static const uint8_t ANIMATION_LOOKUP[] = {
    SPIDER_LARGE_ANIMATION_SLEEP,   /* SpiderLargeStateSleep */
    SPIDER_LARGE_ANIMATION_AWAKEN,  /* SpiderLargeStateAwaken */
    SPIDER_LARGE_ANIMATION_RISE,    /* SpiderLargeStateRise */
    SPIDER_LARGE_ANIMATION_WALK,    /* SpiderLargeStateWalk */
    SPIDER_LARGE_ANIMATION_ATTACK,  /* SpiderLargeStateAttack */
    SPIDER_LARGE_ANIMATION_STAT,    /* SpiderLargeStateStat */
};

typedef uint16_t SpiderLargeState;

static inline void setAnimation(Character *character, int index)
{
    assert(index >= 0 && index < SPIDER_LARGE_ANIMATIONS_COUNT);
    CharacterSetAnimation(character, &SPIDER_LARGE_ANIMATIONS[index]);
}

static inline SpiderLargeState getState(Character *character)
{
    return (SpiderLargeState) character->user1;
}

static inline void setState(Character *character, SpiderLargeState state)
{
    character->user1 = (uint16_t) state;
    character->user2 = 0;
    character->move = 0;
    setAnimation(character, ANIMATION_LOOKUP[state]);
}

static inline void awake(Character *character)
{
    setState(character, SpiderLargeStateAwaken);
}

static inline void rise(Character *character)
{
    setState(character, SpiderLargeStateRise);
}

static inline void walk(Character *character)
{
    setState(character, SpiderLargeStateWalk);
    character->move = 1;
}

static inline void stat(Character *character)
{
    setState(character, SpiderLargeStateStat);
}

static inline void attack(Character *character)
{
    setState(character, SpiderLargeStateAttack);
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
            uint16_t distanceSqr = playerDistanceSqr(self);
            if (distanceSqr < 900)
            {
                awake(self);
            }
            break;
        }
        case SpiderLargeStateAwaken:
        {
            self->user2 += dt;
            if (self->user2 > 500)
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
            uint16_t distanceSqr = playerDistanceSqr(self);
            
            int16_t distance = playerDistanceHor(self);
            self->dir = distance < 0 ? DIR_LEFT : DIR_RIGHT;
            
            if (distanceSqr < 100)
            {
                attack(self);
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

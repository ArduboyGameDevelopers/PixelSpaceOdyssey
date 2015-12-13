#include "spider_large.h"

#include "animation.h"
#include "spider_large_animations.h"

#include "game.h"

#define SpiderLargeStateSleep   SPIDER_LARGE_ANIMATION_SLEEP
#define SpiderLargeStateAwaken  SPIDER_LARGE_ANIMATION_AWAKEN
#define SpiderLargeStateRise    SPIDER_LARGE_ANIMATION_RISE
#define SpiderLargeStateWalk    SPIDER_LARGE_ANIMATION_WALK
#define SpiderLargeStateAttack  SPIDER_LARGE_ANIMATION_ATTACK

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
    character->move = 0;
    setAnimation(character, state);
}

static inline void awake(Character *character)
{
    setState(character, SpiderLargeStateAwaken);
    character->user2 = 0;
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

static inline void attack(Character *character)
{
    setState(character, SpiderLargeStateAttack);
}

void EnemyInitSpiderLarge(Character *character)
{
    setState(character, SpiderLargeStateSleep);
}

void EnemyCallbackSpiderLarge(Character *character, CharacterCallbackType type, int16_t, int16_t)
{
    switch (type)
    {
        case CHARACTER_CALLBACK_ANIMATION_FINISHED:
        {
            SpiderLargeState state = getState(character);
            switch (state)
            {
                case SpiderLargeStateRise:
                    walk(character);
                    break;
                    
                case SpiderLargeStateAttack:
                    walk(character);
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
        {
            uint16_t distanceSqr = playerDistanceSqr(self);
            
            int16_t distance = playerDistanceHor(self);
            self->dir = distance < 0 ? DIR_LEFT : DIR_RIGHT;
            
            if (distanceSqr < 100)
            {
                attack(self);
            }
            break;
        }
        case SpiderLargeStateAttack:
        {
            break;
        }
    }
}

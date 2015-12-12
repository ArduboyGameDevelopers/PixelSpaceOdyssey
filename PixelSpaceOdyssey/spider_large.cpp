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

void EnemyInitSpiderLarge(Character *character)
{
    setState(character, SpiderLargeStateSleep);
}

void EnemyInitSpiderCallback(Character *character, int16_t user1, int16_t user2)
{
    
}

void EnemyBehaviourSpiderLarge(Character *character, TimeInterval dt)
{
    SpiderLargeState state = getState(character);
    
    switch (state)
    {
        case SpiderLargeStateSleep:
        {
            uint16_t distanceSqr = playerDistanceSqr(character);
            if (distanceSqr < 900)
            {
                awake(character);
            }
            break;
        }
        case SpiderLargeStateAwaken:
        {
            character->user2 += dt;
            if (character->user2 > 500)
            {
                rise(character);
            }
            break;
        }
        case SpiderLargeStateRise:
        {
            break;
        }
        case SpiderLargeStateWalk:
        {
            break;
        }
        case SpiderLargeStateAttack:
        {
            break;
        }
    }
}

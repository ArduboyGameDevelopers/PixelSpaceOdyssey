#include "spider_large.h"

#include "animation.h"
#include "spider_large_animations.h"

#define SpiderLargeStateSleep   SPIDER_LARGE_ANIMATION_SLEEP
#define SpiderLargeStateAwaken  SPIDER_LARGE_ANIMATION_AWAKEN
#define SpiderLargeRise         SPIDER_LARGE_ANIMATION_RISE
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

void EnemyInitSpiderLarge(Character *character)
{
    setState(character, SpiderLargeStateSleep);
}

void EnemyBehaviourSpiderLarge(Character *character, TimeInterval dt)
{
}

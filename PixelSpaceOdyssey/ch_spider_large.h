#ifndef SPIDER_LARGE
#define SPIDER_LARGE

#include "character.h"
#include "ch_spider_large_animations.h"

const uint8_t CH_SPIDER_LARGE_ANIMATION_LOOKUP[] = {
    CH_SPIDER_LARGE_ANIMATION_STAT,    /* EnemyStateStat */
    CH_SPIDER_LARGE_ANIMATION_WALK,    /* EnemyStateChase */
    CH_SPIDER_LARGE_ANIMATION_ATTACK,  /* EnemyStateAttack */
    CH_SPIDER_LARGE_ANIMATION_WALK,    /* EnemyStatePatrol */
    CH_SPIDER_LARGE_ANIMATION_SLEEP,   /* EnemyStateSleep */
    CH_SPIDER_LARGE_ANIMATION_AWAKEN,  /* EnemyStateAwaken */
    CH_SPIDER_LARGE_ANIMATION_RISE,    /* EnemyStateRise */
};

extern void EnemyInitSpiderLarge(Character *character);
extern void EnemyCallbackSpiderLarge(Character *character, CharacterCallbackType type, int16_t user1, int16_t user2);
extern void EnemyBehaviourSpiderLarge(Character *character, TimeInterval dt);

inline Character EnemyMakeSpiderLarge()
{
    Character spider = CharacterMake(S2W(CH_SPIDER_LARGE_WIDTH), S2W(CH_SPIDER_LARGE_HEIGHT));
    spider.animations = CH_SPIDER_LARGE_ANIMATIONS;
    spider.animationLookup = CH_SPIDER_LARGE_ANIMATION_LOOKUP;
    spider.init = EnemyInitSpiderLarge;
    spider.behaviour = EnemyBehaviourSpiderLarge;
    spider.callback = EnemyCallbackSpiderLarge;
    spider.sightDistanceForward = 10 * TILE_WIDTH;
    spider.sightDistanceBackward = 4 * TILE_WIDTH;
    return spider;
}

#endif // SPIDER_LARGE


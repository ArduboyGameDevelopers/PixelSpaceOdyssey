#ifndef CH_SPIDER_SMALL_H
#define CH_SPIDER_SMALL_H

#include "character.h"
#include "enemy_base.h"
#include "ch_spider_small_animations.h"

const uint8_t CH_SPIDER_SMALL_ANIMATION_LOOKUP[] = {
    CH_SPIDER_SMALL_ANIMATION_STAT,     /* EnemyStateStat */
    CH_SPIDER_SMALL_ANIMATION_WALK,     /* EnemyStateRun */
    CH_SPIDER_SMALL_ANIMATION_SHOOTWEB, /* EnemyStateAttack */
    CH_SPIDER_SMALL_ANIMATION_WALK      /* EnemyStatePatrol */
};

inline Character EnemyMakeSpiderSmall()
{
    Character dog = CharacterMake(S2W(CH_SPIDER_SMALL_WIDTH), S2W(CH_SPIDER_SMALL_HEIGHT));
    dog.animations = CH_SPIDER_SMALL_ANIMATIONS;
    dog.animationLookup = CH_SPIDER_SMALL_ANIMATION_LOOKUP;
    dog.sightDistanceForward  = 16 * TILE_WIDTH;
    dog.sightDistanceBackward = 16 * TILE_WIDTH;
    dog.init = EnemyDefaultInit;
    dog.behaviour = EnemyDefaultBehaviour;
    dog.callback = EnemyDefaultCallback;
    dog.moveMax = 2;
    return dog;
}

#endif // CH_SPIDER_SMALL_H

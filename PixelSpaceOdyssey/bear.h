#ifndef BEAR
#define BEAR

#include "character.h"
#include "enemy_base.h"
#include "ch_bear_animations.h"

const uint8_t CH_BEAR_ANIMATION_LOOKUP[] = {
    CH_BEAR_ANIMATION_STATICBASE, /* EnemyStateStat */
    CH_BEAR_ANIMATION_WALK,       /* EnemyStateChase */
    CH_BEAR_ANIMATION_ATTACK,     /* EnemyStateAttack */
    CH_BEAR_ANIMATION_WALK,       /* EnemyStatePatrol */
    CH_BEAR_ANIMATION_STATPONC,   /* EnemyStatePonc */
};

extern void EnemyInitBear(Character *character);
extern void EnemyCallbackBear(Character *character, CharacterCallbackType type, int16_t user1, int16_t user2);
extern void EnemyBehaviourBear(Character *character, TimeInterval dt);

inline Character EnemyMakeBearCharacter()
{
    Character bear = CharacterMake(S2W(CH_BEAR_WIDTH), S2W(CH_BEAR_HEIGHT));
    bear.animations = CH_BEAR_ANIMATIONS;
    bear.animationLookup = CH_BEAR_ANIMATION_LOOKUP;
    bear.behaviour = EnemyBehaviourBear;
    bear.callback = EnemyCallbackBear;
    bear.sightDistanceForward  = 10 * TILE_WIDTH;
    bear.sightDistanceBackward = 4 * TILE_WIDTH;
    EnemyDefaultInit(&bear);
    return bear;
}


#endif // BEAR


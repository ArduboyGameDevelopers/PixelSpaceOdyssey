#ifndef DOG
#define DOG

#include "character.h"
#include "enemy_base.h"
#include "ch_dog_animations.h"

const uint8_t CH_DOG_ANIMATION_LOOKUP[] = {
    CH_DOG_ANIMATION_STAT,     /* EnemyStateStat */
    CH_DOG_ANIMATION_RUN,      /* EnemyStateRun */
    CH_DOG_ANIMATION_ATTACK,   /* EnemyStateAttack */
    CH_DOG_ANIMATION_RUN       /* EnemyStatePatrol */
};

extern void EnemyInitDog(Character *character);
extern void EnemyCallbackDog(Character *character, CharacterCallbackType type, int16_t user1, int16_t user2);
extern void EnemyBehaviourDog(Character *character, TimeInterval dt);

inline Character EnemyMakeDog()
{
    Character dog = CharacterMake(S2W(CH_DOG_WIDTH), S2W(CH_DOG_HEIGHT));
    dog.animations = CH_DOG_ANIMATIONS;
    dog.animationLookup = CH_DOG_ANIMATION_LOOKUP;
    dog.sightDistanceForward  = 16 * TILE_WIDTH;
    dog.sightDistanceBackward = 16 * TILE_WIDTH;
    dog.behaviour = EnemyDefaultBehaviour;
    dog.callback = EnemyDefaultCallback;
    dog.moveMax = 3;
    EnemyDefaultInit(&dog);
    return dog;
}

#endif // DOG


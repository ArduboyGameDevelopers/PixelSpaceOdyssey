#ifndef DOG
#define DOG

#include "character.h"
#include "ch_dog_animations.h"

extern void EnemyInitDog(Character *character);
extern void EnemyCallbackDog(Character *character, CharacterCallbackType type, int16_t user1, int16_t user2);
extern void EnemyBehaviourDog(Character *character, TimeInterval dt);

inline Character EnemyMakeDog()
{
    Character character = CharacterMake(S2W(CH_DOG_WIDTH), S2W(CH_DOG_HEIGHT));
    character.sightDistanceForward  = 16 * TILE_WIDTH;
    character.sightDistanceBackward = 16 * TILE_WIDTH;
    character.behaviour = EnemyBehaviourDog;
    character.callback = EnemyCallbackDog;
    EnemyInitDog(&character);
    return character;
}

#endif // DOG


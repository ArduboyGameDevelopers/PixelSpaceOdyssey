#ifndef BEAR
#define BEAR

#include "character.h"
#include "ch_bear_animations.h"

extern void EnemyInitBear(Character *character);
extern void EnemyCallbackBear(Character *character, CharacterCallbackType type, int16_t user1, int16_t user2);
extern void EnemyBehaviourBear(Character *character, TimeInterval dt);

inline Character EnemyMakeBearCharacter()
{
    Character character = CharacterMake(S2W(CH_BEAR_WIDTH), S2W(CH_BEAR_HEIGHT));
    character.behaviour = EnemyBehaviourBear;
    character.callback = EnemyCallbackBear;
    character.sightDistanceForward  = 10 * TILE_WIDTH;
    character.sightDistanceBackward = 4 * TILE_WIDTH;
    EnemyInitBear(&character);
    return character;
}


#endif // BEAR


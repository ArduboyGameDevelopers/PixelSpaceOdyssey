#ifndef BEAR
#define BEAR

#include "character.h"

#define ENEMY_TYPE_BEAR_WIDTH  24
#define ENEMY_TYPE_BEAR_HEIGHT 24
#define ENEMY_TYPE_BEAR_COLLIDER_WIDTH  20
#define ENEMY_TYPE_BEAR_COLLIDER_HEIGHT 16

extern void EnemyInitBear(Character *character);
extern void EnemyCallbackBear(Character *character, CharacterCallbackType type, int16_t user1, int16_t user2);
extern void EnemyBehaviourBear(Character *character, TimeInterval dt);

inline Character EnemyMakeBearCharacter()
{
    int width = S2W(ENEMY_TYPE_BEAR_WIDTH);
    int height = S2W(ENEMY_TYPE_BEAR_HEIGHT);
    Character character = CharacterMake(width, height);
    character.colliderWidth = S2W(ENEMY_TYPE_BEAR_COLLIDER_WIDTH);
    character.colliderHeight = S2W(ENEMY_TYPE_BEAR_COLLIDER_HEIGHT);
    character.behaviour = EnemyBehaviourBear;
    character.callback = EnemyCallbackBear;
    character.sightDistanceForward  = 10 * TILE_WIDTH;
    character.sightDistanceBackward = 4 * TILE_WIDTH;
    EnemyInitBear(&character);
    return character;
}


#endif // BEAR


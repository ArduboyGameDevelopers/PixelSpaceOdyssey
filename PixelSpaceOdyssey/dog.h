#ifndef DOG
#define DOG

#include "character.h"

#define ENEMY_TYPE_DOG_WIDTH  16
#define ENEMY_TYPE_DOG_HEIGHT 10

extern void EnemyInitDog(Character *character);
extern void EnemyCallbackDog(Character *character, CharacterCallbackType type, int16_t user1, int16_t user2);
extern void EnemyBehaviourDog(Character *character, TimeInterval dt);

inline Character EnemyMakeDog()
{
    int width = S2W(ENEMY_TYPE_DOG_WIDTH);
    int height = S2W(ENEMY_TYPE_DOG_HEIGHT);
    Character character = CharacterMake(width, height);
    character.colliderWidth = S2W(ENEMY_TYPE_DOG_WIDTH);
    character.colliderHeight = S2W(ENEMY_TYPE_DOG_HEIGHT);
    character.sightDistanceForward  = 10 * TILE_WIDTH;
    character.sightDistanceBackward = 4 * TILE_WIDTH;
    character.behaviour = EnemyBehaviourDog;
    character.callback = EnemyCallbackDog;
    EnemyInitDog(&character);
    return character;
}

#endif // DOG


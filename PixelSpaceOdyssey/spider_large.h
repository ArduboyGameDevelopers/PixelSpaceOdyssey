#ifndef SPIDER_LARGE
#define SPIDER_LARGE

#include "character.h"

#define ENEMY_TYPE_SPIDER_LARGE_WIDTH  24
#define ENEMY_TYPE_SPIDER_LARGE_HEIGHT 16
#define ENEMY_TYPE_SPIDER_LARGE_COLLIDER_WIDTH  20
#define ENEMY_TYPE_SPIDER_LARGE_COLLIDER_HEIGHT 16

extern void EnemyInitSpiderLarge(Character *character);
extern void EnemyCallbackSpiderLarge(Character *character, CharacterCallbackType type, int16_t user1, int16_t user2);
extern void EnemyBehaviourSpiderLarge(Character *character, TimeInterval dt);

inline Character EnemyMakeSpiderLarge()
{
    int width = S2W(ENEMY_TYPE_SPIDER_LARGE_WIDTH);
    int height = S2W(ENEMY_TYPE_SPIDER_LARGE_HEIGHT);
    Character character = CharacterMake(width, height);
    character.colliderWidth = S2W(ENEMY_TYPE_SPIDER_LARGE_COLLIDER_WIDTH);
    character.colliderHeight = S2W(ENEMY_TYPE_SPIDER_LARGE_COLLIDER_HEIGHT);
    character.behaviour = EnemyBehaviourSpiderLarge;
    character.callback = EnemyCallbackSpiderLarge;
    character.sightDistanceForward  = 10 * TILE_WIDTH;
    character.sightDistanceBackward = 4 * TILE_WIDTH;
    EnemyInitSpiderLarge(&character);
    return character;
}

#endif // SPIDER_LARGE


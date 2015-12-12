#ifndef SPIDER_LARGE
#define SPIDER_LARGE

#include "character.h"

#define ENEMY_TYPE_SPIDER_LARGE_WIDTH  24
#define ENEMY_TYPE_SPIDER_LARGE_HEIGHT 16

extern void EnemyInitSpiderLarge(Character *character);
extern void EnemyInitSpiderCallback(Character *character, int16_t user1, int16_t user2);
extern void EnemyBehaviourSpiderLarge(Character *character, TimeInterval dt);

inline Character EnemyMakeSpiderLargeCharacter()
{
    int width = S2W(ENEMY_TYPE_SPIDER_LARGE_WIDTH);
    int height = S2W(ENEMY_TYPE_SPIDER_LARGE_HEIGHT);
    Character character = CharacterMake(width, height);
    character.behaviour = EnemyBehaviourSpiderLarge;
    EnemyInitSpiderLarge(&character);
    return character;
}

#endif // SPIDER_LARGE


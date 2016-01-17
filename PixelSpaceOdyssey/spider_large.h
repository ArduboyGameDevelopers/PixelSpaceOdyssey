#ifndef SPIDER_LARGE
#define SPIDER_LARGE

#include "character.h"
#include "ch_spider_large_animations.h"

extern void EnemyInitSpiderLarge(Character *character);
extern void EnemyCallbackSpiderLarge(Character *character, CharacterCallbackType type, int16_t user1, int16_t user2);
extern void EnemyBehaviourSpiderLarge(Character *character, TimeInterval dt);

inline Character EnemyMakeSpiderLarge()
{
    Character character = CharacterMake(S2W(CH_SPIDER_LARGE_WIDTH), S2W(CH_SPIDER_LARGE_HEIGHT));
    character.behaviour = EnemyBehaviourSpiderLarge;
    character.callback = EnemyCallbackSpiderLarge;
    character.sightDistanceForward  = 10 * TILE_WIDTH;
    character.sightDistanceBackward = 4 * TILE_WIDTH;
    EnemyInitSpiderLarge(&character);
    return character;
}

#endif // SPIDER_LARGE


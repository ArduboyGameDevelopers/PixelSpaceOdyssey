#ifndef ENEMY_BASE
#define ENEMY_BASE

#include "character.h"

void EnemyUpdate(Character *character, TimeInterval dt);
void UpdateConstraints(Character *character);

inline int16_t EnemyGetMinSightX(const Character *enemy)
{
//    int16_t sightX = enemy->x - enemy->sightDistanceForward;
//    return sightX < enemy->sightMinX ? enemy->sightMinX : sightX;
    return enemy->sightMinX;
}

inline int16_t EnemyGetMaxSightX(const Character *enemy)
{
//    int16_t sightX = enemy->x + enemy->sightDistanceForward;
//    return sightX > enemy->sightMaxX ? enemy->sightMaxX : sightX;
    return enemy->sightMaxX;
}

#endif // ENEMY_BASE


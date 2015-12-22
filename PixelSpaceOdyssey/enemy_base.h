#ifndef ENEMY_BASE
#define ENEMY_BASE

#include "character.h"

void EnemyUpdate(Character *character, TimeInterval dt);
void UpdateConstraints(Character *character);

inline int16_t EnemyGetMinSightX(const Character *enemy)
{
    int16_t sightX = enemy->x - (enemy->dir == DIR_LEFT ? enemy->sightDistanceForward : enemy->sightDistanceBackward);
    return sightX < enemy->sightMinX ? enemy->sightMinX : sightX;
}

inline int16_t EnemyGetMaxSightX(const Character *enemy)
{
    int16_t sightX = enemy->x + (enemy->dir == DIR_RIGHT ? enemy->sightDistanceForward : enemy->sightDistanceBackward);
    return sightX > enemy->sightMaxX ? enemy->sightMaxX : sightX;
}

#endif // ENEMY_BASE


#ifndef ENEMY_BASE
#define ENEMY_BASE

#include "player.h"

#include "Arduboy.h"

#define ENEMY_ATTACK_DELAY 500

void EnemyUpdate(Character *character, TimeInterval dt);
void UpdateConstraints(Character *character);

inline int16_t EnemyGetMinSightX(const Character *self)
{
    int16_t sightX = self->x - (self->dir == DIR_LEFT ? self->sightDistanceForward : self->sightDistanceBackward);
    return sightX < self->sightMinX ? self->sightMinX : sightX;
}

inline int16_t EnemyGetMaxSightX(const Character *self)
{
    int16_t sightX = self->x + (self->dir == DIR_RIGHT ? self->sightDistanceForward : self->sightDistanceBackward);
    return sightX > self->sightMaxX ? self->sightMaxX : sightX;
}

inline bool EnemyCanAttack(const Character *self)
{
    return millis() - self->lastAttackTimestamp > ENEMY_ATTACK_DELAY;
}

inline void EnemyAttack(Character *self)
{
    self->lastAttackTimestamp = millis();
    playerDamage(self);
}

#endif // ENEMY_BASE


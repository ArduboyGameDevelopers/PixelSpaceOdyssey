#ifndef ENEMY_BASE
#define ENEMY_BASE

#include "character.h"
#include "player.h"

#define ENEMY_ATTACK_DELAY 500

// #include "Arduboy.h" - fails to compile on windows
extern unsigned long millis(void);

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

inline bool EnemyCanMoveLeft(const Character *self)
{
    return CharacterGetLeft(self) > self->moveMinX;
}

inline bool EnemyCanMoveRight(const Character *self)
{
    return CharacterGetRight(self) < self->moveMaxX;
}

inline bool EnemyCanMove(const Character *self)
{
    return (self->dir == DIR_LEFT  && EnemyCanMoveLeft(self)) ||
           (self->dir == DIR_RIGHT && EnemyCanMoveRight(self));
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


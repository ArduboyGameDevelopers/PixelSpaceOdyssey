#ifndef ENEMY_BASE
#define ENEMY_BASE

#include "character.h"
#include "player.h"

#define ENEMY_ATTACK_DELAY 500

// #include "Arduboy.h" - fails to compile on windows
extern unsigned long millis(void);
extern long random(long);
extern long random(long, long);

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

/// Time elasped since last attack is enough for a new attack
inline bool EnemyCanAttack(const Character *self)
{
    return millis() - self->lastAttackTimestamp > ENEMY_ATTACK_DELAY;
}

/// Enemy is close enought to the last player's position to attack
inline bool EnemyIsCloseToAttack(const Character *self)
{
    return ABS(self->lastPlayerX - self->x) <= DIV2(self->width + player.width);
}

inline void EnemyAttack(Character *self)
{
    self->lastAttackTimestamp = millis();
    playerDamage(self);
}

inline void EnemySetTargetPos(Character *self, int16_t target)
{
    self->targetPos = target;
    self->hasTarget = true;
}

#endif // ENEMY_BASE


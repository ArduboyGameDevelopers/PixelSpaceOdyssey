#ifndef ENEMY_BASE
#define ENEMY_BASE

#include "character.h"
#include "player.h"

#define ENEMY_ATTACK_DELAY 500

// #include "Arduboy.h" - fails to compile on windows
extern unsigned long millis(void);
extern long random(long);
extern long random(long, long);

#define EnemyStateStat    0
#define EnemyStateChase   1
#define EnemyStateAttack  2
#define EnemyStatePatrol  3

typedef uint16_t EnemyState;
typedef uint8_t  EnemyType;

/** Returns enemy state */
inline EnemyState EnemyGetState(Character *self)
{
    return (EnemyState) self->state;
}

/** Update function for enemy */
void EnemyUpdate(Character *character, TimeInterval dt);

/** Update enemy move and sight constraints */
void UpdateConstraints(Character *character);

/** The farthest point enemy can see on the left */
inline int16_t EnemyGetMinSightX(const Character *self)
{
    int16_t sightX = self->x - (self->dir == DIR_LEFT ? self->sightDistanceForward : self->sightDistanceBackward);
    return sightX < self->sightMinX ? self->sightMinX : sightX;
}

/** The farthest point enemy can see on the right */
inline int16_t EnemyGetMaxSightX(const Character *self)
{
    int16_t sightX = self->x + (self->dir == DIR_RIGHT ? self->sightDistanceForward : self->sightDistanceBackward);
    return sightX > self->sightMaxX ? self->sightMaxX : sightX;
}

/** True if there's no obstacle on the enemy's left */
inline bool EnemyCanMoveLeft(const Character *self)
{
    return CharacterGetLeft(self) > self->moveMinX;
}

/** True if there's no obstacle on the enemy's right */
inline bool EnemyCanMoveRight(const Character *self)
{
    return CharacterGetRight(self) < self->moveMaxX;
}

/** True if enemy can move into the direction it's currently facing */
inline bool EnemyCanMove(const Character *self)
{
    return (self->dir == DIR_LEFT  && EnemyCanMoveLeft(self)) ||
           (self->dir == DIR_RIGHT && EnemyCanMoveRight(self));
}

/** True if enought time passed since the last enemy's attack  */
inline bool EnemyCanAttack(const Character *self)
{
    return millis() - self->lastAttackTimestamp > ENEMY_ATTACK_DELAY;
}

/** Enemy is close enought to the last player's position to attack */
inline bool EnemyIsCloseToAttack(const Character *self)
{
    return ABS(self->lastPlayerX - self->x) <= DIV2(self->width + player.width);
}

/** Initiates an attack to the player */
inline void EnemyAttackBaseBase(Character *self)
{
    self->lastAttackTimestamp = millis();
    playerDamage(self);
}

/** Set enemy's target position */
inline void EnemySetTargetPos(Character *self, int16_t target)
{
    self->targetPos = target;
    self->hasTarget = true;
}

#endif // ENEMY_BASE


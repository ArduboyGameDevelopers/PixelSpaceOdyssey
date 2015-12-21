#ifndef ENEMY_BASE
#define ENEMY_BASE

#include "character.h"

void EnemyUpdate(Character *character, TimeInterval dt);
void UpdateConstraints(Character *character);

#endif // ENEMY_BASE


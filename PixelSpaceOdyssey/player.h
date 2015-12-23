#ifndef PLAYER
#define PLAYER

#include "character.h"

extern Character player;

void playerUpdate(TimeInterval dt);
void playerDraw();
void playerDamage(Character *enemy);

#endif // PLAYER


#ifndef PLAYER
#define PLAYER

#include "character.h"

extern Character player;

void playerUpdate(TimeInterval dt);
void playerDraw();

#endif // PLAYER


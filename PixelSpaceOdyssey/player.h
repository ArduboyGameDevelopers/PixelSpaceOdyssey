#ifndef PLAYER
#define PLAYER

#include "character.h"

extern const int16_t kPlayerAmmoMax;
extern const int16_t kPlayerHealthMax;

extern Character player;
extern uint8_t playerHealth;

void createPlayer();
void playerUpdate(TimeInterval dt);
void playerDraw();
void playerDamage(Character *enemy);

#endif // PLAYER


#ifndef PLAYER
#define PLAYER

#include "character.h"

extern const int16_t kPlayerAmmoMax;
extern const int16_t kPlayerHealthMax;

extern Character player;
extern uint8_t playerHealth;
extern uint8_t playerAmmo;

/* Enemy AI will target this position to create some delay */
extern int16_t playerLastSeenX;
extern int16_t playerLastSeenY;

void playerDamage(Character *enemy);
void playerDie();

#endif // PLAYER


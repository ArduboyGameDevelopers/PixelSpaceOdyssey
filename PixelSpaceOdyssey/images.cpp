#include <avr/pgmspace.h>

#include "images.h"

PROGMEM WEAK_CONST unsigned char IMG_HUD_GUN[] =
{
  7, 4,
  0x3, 0xf, 0xf, 0x7, 0x3, 0x3, 0x3
};

PROGMEM WEAK_CONST unsigned char IMG_HUD_HEART[] =
{
  5, 4,
  0x3, 0x7, 0xe, 0x7, 0x3
};

PROGMEM WEAK_CONST unsigned char IMG_HUD_LOGO[] =
{
  16, 7,
  0x7f, 0x41, 0x75, 0x75, 0x51, 0x7f, 0x51, 0x55, 0x55, 0x45, 0x7f, 0x41, 0x5d, 0x5d, 0x41, 0x7f
};

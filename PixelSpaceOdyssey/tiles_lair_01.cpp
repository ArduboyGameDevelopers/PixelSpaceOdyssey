#include <avr/pgmspace.h>

#include "tiles_lair_01.h"
#include "common.h"

PROGMEM static const unsigned char TILES_LAIR_01_1[] = 
{
  0x80, 0xa0, 0xe0, 0xf4, 0xdc, 0x6e, 0xbe, 0xf7
};

PROGMEM static const unsigned char TILES_LAIR_01_2[] = 
{
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};

PROGMEM static const unsigned char TILES_LAIR_01_3[] = 
{
  0x7f, 0x0f, 0x17, 0x07, 0x4b, 0x3f, 0x1f, 0x03
};

PROGMEM static const unsigned char TILES_LAIR_01_4[] = 
{
  0x9d, 0x59, 0x67, 0x24, 0x3f, 0x1a, 0x0a, 0x0f
};

PROGMEM static const unsigned char TILES_LAIR_01_5[] = 
{
  0x05, 0x05, 0x02, 0x03, 0x02, 0x02, 0x01, 0x01
};

PROGMEM static const unsigned char TILES_LAIR_01_6[] = 
{
  0x0f, 0x63, 0xff, 0xcf, 0xff, 0x7f, 0xff, 0xff
};

PROGMEM static const unsigned char TILES_LAIR_01_7[] = 
{
  0x7f, 0x3f, 0x1f, 0x7f, 0x1b, 0x0f, 0x03, 0x0f
};

PROGMEM static const unsigned char TILES_LAIR_01_8[] = 
{
  0xff, 0xfe, 0xf4, 0xe4, 0xfc, 0xb0, 0xe0, 0xc0
};

PROGMEM static const unsigned char TILES_LAIR_01_9[] = 
{
  0x01, 0x01, 0x01, 0x03, 0x03, 0x05, 0x05, 0x0b
};

PROGMEM static const unsigned char TILES_LAIR_01_10[] = 
{
  0x0f, 0x1f, 0x17, 0x27, 0x33, 0x3b, 0x7b, 0xfd
};

PROGMEM static const unsigned char TILES_LAIR_01_11[] = 
{
  0xed, 0xaf, 0x33, 0x33, 0xbf, 0xcd, 0xef, 0x7b
};

PROGMEM static const unsigned char TILES_LAIR_01_12[] = 
{
  0xff, 0x9f, 0xef, 0x7f, 0x5f, 0xff, 0xff, 0xff
};

PROGMEM static const unsigned char TILES_LAIR_01_13[] = 
{
  0x03, 0x0f, 0x7f, 0x07, 0x07, 0x0d, 0x18, 0xfe
};

PROGMEM static const unsigned char TILES_LAIR_01_14[] = 
{
  0xff, 0xc3, 0xc1, 0xb3, 0xfe, 0x47, 0xc9, 0xff
};

PROGMEM static const unsigned char TILES_LAIR_01_15[] = 
{
  0x0f, 0x03, 0x0f, 0x1b, 0x7f, 0x1f, 0x3f, 0x7f
};

PROGMEM static const unsigned char TILES_LAIR_01_16[] = 
{
  0x07, 0x00, 0x01, 0x03, 0x0f, 0x0f, 0x38, 0xff
};

PROGMEM static const unsigned char TILES_LAIR_01_17[] = 
{
  0xd0, 0xf0, 0xf8, 0xff, 0xe0, 0xe0, 0xfe, 0xf8
};

PROGMEM static const unsigned char TILES_LAIR_01_18[] = 
{
  0xf8, 0x9c, 0x84, 0xc2, 0xe0, 0xf0, 0x98, 0xcc
};

PgmPtr const TILES_LAIR_01[] =
{
  TILES_LAIR_01_1,
  TILES_LAIR_01_2,
  TILES_LAIR_01_3,
  TILES_LAIR_01_4,
  TILES_LAIR_01_5,
  TILES_LAIR_01_6,
  TILES_LAIR_01_7,
  TILES_LAIR_01_8,
  TILES_LAIR_01_9,
  TILES_LAIR_01_10,
  TILES_LAIR_01_11,
  TILES_LAIR_01_12,
  TILES_LAIR_01_13,
  TILES_LAIR_01_14,
  TILES_LAIR_01_15,
  TILES_LAIR_01_16,
  TILES_LAIR_01_17,
  TILES_LAIR_01_18
};

const uint8_t INDICES_LAIR_01[] =
{
  0,0,0,1,2,3,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,1,2,4,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,6,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  1,3,0,0,0,0,0,0,0,0,0,0,0,0,1,2,2,8,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,2,2,2,8,
  2,3,0,0,0,0,0,0,0,0,0,0,0,9,10,11,12,2,2,
  6,8,0,0,0,0,0,0,0,0,0,0,0,0,0,13,14,2,2,
  15,2,2,8,0,0,0,0,0,0,0,0,0,0,0,0,16,2,2,
  1,2,2,2,8,0,0,0,0,0,0,0,0,0,0,0,0,15,2,
  2,2,2,2,2,17,18,0,0,0,0,0,0,0,0,0,0,1,2,
  2,2,2,2,2,2,2,2,2,2,2,2,2,8,0,0,1,2,2,
  2,2,2,2,2,2,2,2,2,2,2,2,2,2,8,0,15,2,2,
  2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,8,0,15,2,
};

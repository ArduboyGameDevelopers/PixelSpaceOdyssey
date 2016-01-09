#include <avr/pgmspace.h>

#include "tiles_tb_caverns.h"
#include "common.h"

PROGMEM static WEAK_CONST unsigned char TILES_TB_CAVERNS_THIN_1[] = 
{
  0x00, 0x00, 0x80, 0x80, 0x00, 0x80, 0x00, 0x00
};

PROGMEM static WEAK_CONST unsigned char TILES_TB_CAVERNS_THIN_2[] = 
{
  0x7f, 0x0f, 0x17, 0x07, 0x4b, 0x3f, 0x1f, 0x03
};

PROGMEM static WEAK_CONST unsigned char TILES_TB_CAVERNS_THIN_3[] = 
{
  0x0f, 0x03, 0x0f, 0x1b, 0x7f, 0x1f, 0x3f, 0x7f
};

PROGMEM static WEAK_CONST unsigned char TILES_TB_CAVERNS_THIN_4[] = 
{
  0x6f, 0xcb, 0x9f, 0x36, 0xee, 0xdd, 0xbb, 0x36
};

PROGMEM static WEAK_CONST unsigned char TILES_TB_CAVERNS_THIN_5[] = 
{
  0xed, 0x79, 0x77, 0x25, 0x3f, 0x7b, 0x0b, 0x0f
};

PROGMEM static WEAK_CONST unsigned char TILES_TB_CAVERNS_THIN_6[] = 
{
  0x05, 0x05, 0x02, 0x03, 0x02, 0x03, 0x01, 0x01
};

PROGMEM static WEAK_CONST unsigned char TILES_TB_CAVERNS_THIN_7[] = 
{
  0x7f, 0x34, 0x09, 0x0d, 0x06, 0x3e, 0x03, 0x01
};

PROGMEM static WEAK_CONST unsigned char TILES_TB_CAVERNS_THIN_8[] = 
{
  0x01, 0x01, 0x01, 0x03, 0x03, 0x05, 0x05, 0x0b
};

PROGMEM static WEAK_CONST unsigned char TILES_TB_CAVERNS_THIN_9[] = 
{
  0x0f, 0x1f, 0x17, 0x27, 0x33, 0x3b, 0x7b, 0xfd
};

PROGMEM static WEAK_CONST unsigned char TILES_TB_CAVERNS_THIN_10[] = 
{
  0xed, 0xaf, 0x33, 0x33, 0xbf, 0xcd, 0xef, 0x7b
};

PROGMEM static WEAK_CONST unsigned char TILES_TB_CAVERNS_THIN_11[] = 
{
  0x03, 0x0f, 0x7f, 0x07, 0x07, 0x0d, 0x18, 0xfe
};

PROGMEM static WEAK_CONST unsigned char TILES_TB_CAVERNS_THIN_12[] = 
{
  0x01, 0x03, 0xfd, 0xdf, 0x27, 0x1f, 0x0f, 0x0b
};

PROGMEM static WEAK_CONST unsigned char TILES_TB_CAVERNS_THIN_13[] = 
{
  0x00, 0x00, 0x80, 0x47, 0x3c, 0x00, 0x00, 0x00
};

PROGMEM static WEAK_CONST unsigned char TILES_TB_CAVERNS_SLOPES_RIGHT[] = 
{
  0xff, 0xfe, 0xf4, 0xe4, 0xfc, 0xb0, 0xe0, 0xc0
};

PROGMEM static WEAK_CONST unsigned char TILES_TB_CAVERNS_SLOPES_LEFT[] = 
{
  0x80, 0xa0, 0xe0, 0xf4, 0xdc, 0x6e, 0xbe, 0xf7
};

PROGMEM static WEAK_CONST unsigned char TILES_TB_CAVERNS_SOLID_1[] = 
{
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};

PROGMEM static WEAK_CONST unsigned char TILES_TB_CAVERNS_SOLID_2[] = 
{
  0x0f, 0x63, 0xff, 0xcf, 0xff, 0x7f, 0xff, 0xff
};

PROGMEM static WEAK_CONST unsigned char TILES_TB_CAVERNS_SOLID_3[] = 
{
  0xff, 0xbf, 0xff, 0xfb, 0xe7, 0xef, 0xfb, 0x21
};

PROGMEM static WEAK_CONST unsigned char TILES_TB_CAVERNS_SOLID_4[] = 
{
  0xff, 0xff, 0xfe, 0xff, 0xff, 0xfe, 0xfe, 0xff
};

PROGMEM static WEAK_CONST unsigned char TILES_TB_CAVERNS_SOLID_5[] = 
{
  0x01, 0x03, 0x03, 0x06, 0x0f, 0x1e, 0x3f, 0x3f
};

PROGMEM static WEAK_CONST unsigned char TILES_TB_CAVERNS_SOLID_6[] = 
{
  0x1f, 0x07, 0x7f, 0x1d, 0x3f, 0x2d, 0x6d, 0x2f
};

PROGMEM static WEAK_CONST unsigned char TILES_TB_CAVERNS_SOLID_7[] = 
{
  0x3b, 0x1b, 0x1f, 0x7f, 0x07, 0x1f, 0x3f, 0x1f
};

PROGMEM static WEAK_CONST unsigned char TILES_TB_CAVERNS_SOLID_8[] = 
{
  0x0f, 0x07, 0x07, 0x05, 0x03, 0x01, 0x01, 0x01
};

PROGMEM static WEAK_CONST unsigned char TILES_TB_CAVERNS_SPAWN_1[] = 
{
  0xc1, 0x9f, 0x83, 0x8f, 0xc7, 0xff, 0xcf, 0xff
};

PROGMEM static WEAK_CONST unsigned char TILES_TB_CAVERNS_SPAWN_2[] = 
{
  0xff, 0x8f, 0xff, 0xc7, 0xe3, 0x83, 0x81, 0x83
};

PROGMEM static WEAK_CONST unsigned char TILES_TB_CAVERNS_HAZARD_1[] = 
{
  0xd0, 0xf0, 0xf8, 0xff, 0xe0, 0xe0, 0xfe, 0xf8
};

PROGMEM static WEAK_CONST unsigned char TILES_TB_CAVERNS_HAZARD_2[] = 
{
  0xf8, 0x9c, 0x84, 0xc2, 0xe0, 0xf0, 0x98, 0xcc
};

PROGMEM static WEAK_CONST unsigned char TILES_TB_CAVERNS_HAZARD_3[] = 
{
  0xff, 0xff, 0xdc, 0xe0, 0xf0, 0xf6, 0xef, 0xff
};

PROGMEM static WEAK_CONST unsigned char TILES_TB_CAVERNS_HAZARD_4[] = 
{
  0xff, 0xcf, 0xe2, 0xf8, 0xe0, 0xfc, 0xf0, 0x80
};

PROGMEM static WEAK_CONST unsigned char TILES_TB_CAVERNS_HAZARD_5[] = 
{
  0xf8, 0xc0, 0xf0, 0xc0, 0xf0, 0xff, 0xfc, 0xf0
};

PROGMEM static WEAK_CONST unsigned char TILES_TB_CAVERNS_HAZARD_6[] = 
{
  0xff, 0xfe, 0x80, 0xf8, 0xfc, 0xe0, 0xfc, 0xff
};

PROGMEM static WEAK_CONST unsigned char TILES_TB_CAVERNS_ITEMS_1[] = 
{
  0x00, 0x7c, 0x6c, 0x44, 0x6c, 0x7c, 0x00, 0x00
};

PROGMEM static WEAK_CONST unsigned char TILES_TB_CAVERNS_ITEMS_2[] = 
{
  0x00, 0x18, 0x00, 0x18, 0x00, 0x18, 0x00, 0x00
};

PROGMEM static WEAK_CONST unsigned char TILES_TB_CAVERNS_ITEMS_3[] = 
{
  0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00
};

PgmPtr WEAK_CONST TILES_TB_CAVERNS[] =
{
  TILES_TB_CAVERNS_THIN_1 /* 1 */,
  TILES_TB_CAVERNS_THIN_2 /* 2 */,
  TILES_TB_CAVERNS_THIN_3 /* 3 */,
  TILES_TB_CAVERNS_THIN_4 /* 4 */,
  TILES_TB_CAVERNS_THIN_5 /* 5 */,
  TILES_TB_CAVERNS_THIN_6 /* 6 */,
  TILES_TB_CAVERNS_THIN_7 /* 7 */,
  TILES_TB_CAVERNS_THIN_8 /* 8 */,
  TILES_TB_CAVERNS_THIN_9 /* 9 */,
  TILES_TB_CAVERNS_THIN_10 /* 10 */,
  TILES_TB_CAVERNS_THIN_11 /* 11 */,
  TILES_TB_CAVERNS_THIN_12 /* 12 */,
  TILES_TB_CAVERNS_THIN_13 /* 13 */,
  TILES_TB_CAVERNS_SLOPES_RIGHT /* 14 */,
  TILES_TB_CAVERNS_SLOPES_LEFT /* 15 */,
  TILES_TB_CAVERNS_SOLID_1 /* 16 */,
  TILES_TB_CAVERNS_SOLID_2 /* 17 */,
  TILES_TB_CAVERNS_SOLID_3 /* 18 */,
  TILES_TB_CAVERNS_SOLID_4 /* 19 */,
  TILES_TB_CAVERNS_SOLID_5 /* 20 */,
  TILES_TB_CAVERNS_SOLID_6 /* 21 */,
  TILES_TB_CAVERNS_SOLID_7 /* 22 */,
  TILES_TB_CAVERNS_SOLID_8 /* 23 */,
  TILES_TB_CAVERNS_SPAWN_1 /* 24 */,
  TILES_TB_CAVERNS_SPAWN_2 /* 25 */,
  TILES_TB_CAVERNS_HAZARD_1 /* 26 */,
  TILES_TB_CAVERNS_HAZARD_2 /* 27 */,
  TILES_TB_CAVERNS_HAZARD_3 /* 28 */,
  TILES_TB_CAVERNS_HAZARD_4 /* 29 */,
  TILES_TB_CAVERNS_HAZARD_5 /* 30 */,
  TILES_TB_CAVERNS_HAZARD_6 /* 31 */,
  TILES_TB_CAVERNS_ITEMS_1 /* 32 */,
  TILES_TB_CAVERNS_ITEMS_2 /* 33 */,
  TILES_TB_CAVERNS_ITEMS_3 /* 34 */
};


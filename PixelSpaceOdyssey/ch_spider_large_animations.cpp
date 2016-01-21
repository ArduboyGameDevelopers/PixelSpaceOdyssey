#include <avr/pgmspace.h>

#include "ch_spider_large_animations.h"

static PROGMEM WEAK_CONST unsigned char FRAME_ATTACK_0[] =
{
  22, 15, -10, -7, -12, -8, 
  0xc0, 0x40, 0x80, 0x80, 0xfe, 0xc6, 0xec, 0xf0, 0xf3, 0xfe, 0xfc, 0xfc, 0xf8, 0xf0, 0xf8, 0x6, 0x1, 0x6, 0x8, 0x10, 0x60, 0x80, 0x7, 0x78, 0x1, 0xf, 0xf, 0x7f, 0xf, 0xf, 0x7, 0x7, 0x7, 0x6, 0x1f, 0x6e, 0x7, 0xc, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0
};

static PROGMEM WEAK_CONST unsigned char FRAME_ATTACK_1[] =
{
  23, 16, -9, -8, -14, -8, 
  0x0, 0x0, 0x0, 0x80, 0x0, 0x0, 0x80, 0xf8, 0xb0, 0xc0, 0xe0, 0xe0, 0xe0, 0xe0, 0xfc, 0xe3, 0xe2, 0xec, 0xd0, 0xa0, 0xc0, 0x0, 0x0, 0xe0, 0x18, 0x7, 0x0, 0xc1, 0x3d, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0xff, 0x1f, 0x1f, 0x1f, 0xf, 0xd, 0xf, 0x1d, 0x17, 0x1, 0x2
};

static PgmPtr FRAMES_ATTACK[] =
{
  FRAME_ATTACK_0,
  FRAME_ATTACK_1,
};

static PROGMEM WEAK_CONST unsigned char FRAME_WALK_0[] =
{
  24, 14, -10, -6, -14, -8, 
  0x0, 0x0, 0xc0, 0x80, 0x80, 0xf0, 0xff, 0xf3, 0xfe, 0xf8, 0xf8, 0xf0, 0xf0, 0xfe, 0xf8, 0xe0, 0xf0, 0x8, 0x4, 0x1c, 0x60, 0x80, 0x0, 0x0, 0x38, 0x7, 0x0, 0x0, 0x3f, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x37, 0xf, 0x5, 0x7, 0x1d, 0xf, 0x18, 0x0, 0x0, 0x0, 0x1, 0x6, 0x18
};

static PROGMEM WEAK_CONST unsigned char FRAME_WALK_1[] =
{
  24, 15, -10, -7, -14, -8, 
  0x0, 0x0, 0x0, 0xc0, 0x80, 0x80, 0xfe, 0xc3, 0xec, 0xf0, 0xe0, 0xf0, 0xf0, 0xfe, 0xf8, 0xe0, 0xc0, 0x40, 0x60, 0x70, 0xc0, 0x0, 0x0, 0x0, 0x70, 0xc, 0x3, 0x1, 0xf, 0x3f, 0xf, 0xf, 0xf, 0xf, 0x6f, 0x1f, 0x7, 0x5, 0x7, 0x1d, 0xf, 0x18, 0x0, 0x0, 0x0, 0x7, 0x18, 0x60
};

static PROGMEM WEAK_CONST unsigned char FRAME_WALK_2[] =
{
  23, 13, -10, -5, -13, -8, 
  0x40, 0x20, 0x18, 0x74, 0xe0, 0xe0, 0xff, 0xfe, 0xf8, 0xf8, 0xf0, 0xf8, 0xf8, 0xff, 0xfc, 0xf0, 0xe0, 0x20, 0x30, 0x1c, 0x70, 0x80, 0x0, 0x0, 0x0, 0x0, 0x0, 0x3, 0x1f, 0x3, 0x3, 0x3, 0x1f, 0x7, 0x7, 0x3, 0x2, 0x3, 0xe, 0x7, 0xc, 0x0, 0x0, 0x0, 0x7, 0x18
};

static PROGMEM WEAK_CONST unsigned char FRAME_WALK_3[] =
{
  20, 14, -9, -6, -11, -8, 
  0x0, 0xf0, 0xf8, 0xc0, 0xc0, 0xfe, 0xfc, 0xf0, 0xf0, 0xf0, 0xf8, 0xf8, 0xff, 0xfc, 0xf0, 0xe0, 0x20, 0x60, 0xf8, 0x0, 0x38, 0x7, 0x30, 0xf, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x1f, 0x2, 0x3, 0xe, 0x7, 0xc, 0x0, 0x1, 0x3e
};

static PROGMEM WEAK_CONST unsigned char FRAME_WALK_4[] =
{
  19, 13, -9, -5, -10, -8, 
  0x0, 0xf8, 0x70, 0xe0, 0xe0, 0xf8, 0xfe, 0xff, 0xf8, 0xf8, 0xf8, 0xf8, 0xfe, 0xfc, 0xf0, 0xe0, 0x10, 0xf0, 0x3c, 0x1e, 0x1, 0x0, 0x3, 0x7, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x1f, 0x2, 0x3, 0xe, 0x7, 0xc, 0x1f, 0x0
};

static PgmPtr FRAMES_WALK[] =
{
  FRAME_WALK_0,
  FRAME_WALK_1,
  FRAME_WALK_2,
  FRAME_WALK_3,
  FRAME_WALK_4,
};

static PROGMEM WEAK_CONST unsigned char FRAME_STAT_0[] =
{
  18, 14, -9, -6, -9, -8, 
  0xe0, 0x20, 0x40, 0xc0, 0xff, 0xe3, 0xf6, 0xf8, 0xf0, 0xf0, 0xf0, 0xfe, 0xf8, 0xe0, 0xc0, 0x20, 0x10, 0xf8, 0x3f, 0x0, 0x7, 0x7, 0x3f, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x3d, 0x7, 0x1d, 0xf, 0x18, 0x0, 0x3f
};

static PgmPtr FRAMES_STAT[] =
{
  FRAME_STAT_0,
};

static PROGMEM WEAK_CONST unsigned char FRAME_RISE_0[] =
{
  17, 11, -9, -3, -8, -8, 
  0xf0, 0x60, 0xc0, 0xc0, 0xe0, 0xd8, 0xe6, 0xff, 0xe0, 0xff, 0xfe, 0xf8, 0xf8, 0xfe, 0xf8, 0x20, 0xc0, 0x7, 0x0, 0x6, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x5, 0x7, 0x5, 0x7, 0x0, 0x7
};

static PROGMEM WEAK_CONST unsigned char FRAME_RISE_1[] =
{
  18, 13, -9, -5, -9, -8, 
  0x80, 0x80, 0xe0, 0x98, 0xd8, 0xc6, 0xff, 0xf0, 0xf0, 0xe0, 0xec, 0xff, 0xfc, 0xc0, 0xf0, 0x1c, 0xe0, 0x0, 0x1f, 0x0, 0xf, 0xf, 0x1f, 0xf, 0xf, 0xf, 0xf, 0xf, 0xf, 0x1b, 0xf, 0x1b, 0x1f, 0x10, 0x3, 0x1c
};

static PROGMEM WEAK_CONST unsigned char FRAME_RISE_2[] =
{
  18, 14, -9, -6, -9, -8, 
  0xe0, 0x20, 0x40, 0xc0, 0xff, 0xe3, 0xf6, 0xf8, 0xf0, 0xf0, 0xf0, 0xfe, 0xf8, 0xe0, 0xc0, 0x20, 0x10, 0xf8, 0x3f, 0x0, 0x7, 0x7, 0x3f, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x3d, 0x7, 0x1d, 0xf, 0x18, 0x0, 0x3f
};

static PgmPtr FRAMES_RISE[] =
{
  FRAME_RISE_0,
  FRAME_RISE_1,
  FRAME_RISE_2,
};

static PROGMEM WEAK_CONST unsigned char FRAME_AWAKEN_0[] =
{
  15, 11, -7, -3, -8, -8, 
  0x0, 0x0, 0xc0, 0xc0, 0xf0, 0xfc, 0xff, 0xc0, 0xff, 0xe6, 0xf8, 0xf0, 0xc0, 0x80, 0x0, 0x4, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x3, 0x7, 0x3, 0x7, 0x7, 0x4
};

static PgmPtr FRAMES_AWAKEN[] =
{
  FRAME_AWAKEN_0,
};

static PROGMEM WEAK_CONST unsigned char FRAME_SLEEP_0[] =
{
  15, 11, -7, -3, -8, -8, 
  0x0, 0x0, 0xc0, 0xc0, 0xf0, 0xfc, 0xff, 0xc0, 0xff, 0xe6, 0xf8, 0xf0, 0xc0, 0x80, 0x0, 0x4, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x4
};

static PgmPtr FRAMES_SLEEP[] =
{
  FRAME_SLEEP_0,
};

const Animation CH_SPIDER_LARGE_ANIMATIONS[] = 
{
  AnimationMake(FRAMES_ATTACK, 2, 24, 16),
  AnimationMake(FRAMES_WALK, 5, 24, 16, true),
  AnimationMake(FRAMES_STAT, 1, 24, 16),
  AnimationMake(FRAMES_RISE, 3, 24, 16),
  AnimationMake(FRAMES_AWAKEN, 1, 24, 16),
  AnimationMake(FRAMES_SLEEP, 1, 24, 16),
};
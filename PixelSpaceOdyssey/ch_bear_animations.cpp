#include <avr/pgmspace.h>

#include "ch_bear_animations.h"

static PROGMEM WEAK_CONST unsigned char FRAME_DEATH_0[] =
{
  2, 8, 18, 16,
  0xe0, 0xf0, 0xf8, 0xfc, 0xfc, 0xfc, 0xfe, 0xfe, 0xff, 0xff, 0xfd, 0xff, 0xcf, 0xc7, 0xc0, 0xc0, 0x80, 0x80, 0x7, 0x8f, 0xdf, 0xdf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xbb, 0x3, 0x3, 0x6, 0xc
};

static PROGMEM WEAK_CONST unsigned char FRAME_DEATH_1[] =
{
  3, 12, 17, 12,
  0xe0, 0xff, 0xfe, 0xfe, 0xff, 0xdf, 0xff, 0xff, 0xfe, 0xfc, 0xfc, 0xbc, 0x38, 0x28, 0x24, 0x0, 0x0, 0x3, 0xf, 0xf, 0xf, 0xf, 0xf, 0xf, 0xf, 0xf, 0xf, 0xf, 0xf, 0xf, 0xb, 0xe, 0xd, 0xe
};

static PROGMEM WEAK_CONST unsigned char FRAME_DEATH_2[] =
{
  4, 18, 17, 6,
  0x20, 0x3e, 0x3e, 0x2f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3c, 0x38, 0x38, 0x10, 0x30, 0x20
};

static PgmPtr FRAMES_DEATH[] =
{
  FRAME_DEATH_0,
  FRAME_DEATH_1,
  FRAME_DEATH_2,
};

static PROGMEM WEAK_CONST unsigned char FRAME_WALK_0[] =
{
  1, 8, 20, 16,
  0x0, 0xc0, 0xf0, 0xf8, 0xfc, 0xfc, 0xfe, 0xfe, 0xfe, 0xff, 0xff, 0xfe, 0xfe, 0xfe, 0xf6, 0xfe, 0xc0, 0x0, 0x0, 0x0, 0x10, 0x3f, 0x1f, 0x7f, 0x0, 0x39, 0x79, 0x7f, 0xff, 0xbf, 0xe7, 0xff, 0xbf, 0x1, 0x1, 0xf, 0x3f, 0x1c, 0x18, 0x10
};

static PROGMEM WEAK_CONST unsigned char FRAME_WALK_1[] =
{
  2, 9, 16, 15,
  0x0, 0xfc, 0xfc, 0xfe, 0xfe, 0x3e, 0xfe, 0xff, 0xff, 0xfe, 0xfe, 0xfc, 0xfc, 0xec, 0x7c, 0x0, 0x8, 0x7, 0x13, 0xf, 0x47, 0x70, 0x6f, 0x5f, 0x3f, 0x7f, 0x7f, 0x79, 0x43, 0x1f, 0xf, 0x18
};

static PROGMEM WEAK_CONST unsigned char FRAME_WALK_2[] =
{
  1, 9, 17, 15,
  0x0, 0x0, 0xf8, 0xfc, 0xfe, 0x7e, 0x9f, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xfe, 0xfe, 0xf6, 0x7e, 0x0, 0x2, 0x3, 0x3, 0x3, 0x7, 0x0, 0x2f, 0x7f, 0x7f, 0x7f, 0x5f, 0x3f, 0x3d, 0x3, 0xf, 0x7, 0x1c
};

static PROGMEM WEAK_CONST unsigned char FRAME_WALK_3[] =
{
  1, 9, 19, 15,
  0x0, 0xe0, 0xf0, 0xf8, 0x7c, 0xfc, 0xfe, 0xfe, 0xff, 0xff, 0xfe, 0xfe, 0xfc, 0xfc, 0xec, 0xfc, 0xc0, 0x80, 0x0, 0x8, 0xf, 0x7, 0x1f, 0x40, 0x7f, 0x7f, 0x7f, 0x3f, 0xf, 0x7f, 0x7f, 0x7f, 0x70, 0x0, 0x0, 0x1b, 0xf, 0x3f
};

static PgmPtr FRAMES_WALK[] =
{
  FRAME_WALK_0,
  FRAME_WALK_1,
  FRAME_WALK_2,
  FRAME_WALK_3,
};

static PROGMEM WEAK_CONST unsigned char FRAME_ATTACK_0[] =
{
  0, 0, 17, 24,
  0xff, 0xfc, 0xef, 0x89, 0x8, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x3, 0x1f, 0xff, 0xff, 0xfe, 0xff, 0xfb, 0xff, 0xfe, 0xfe, 0xfe, 0xf0, 0x70, 0xe0, 0xe0, 0x60, 0x0, 0x0, 0x0, 0x1f, 0x3f, 0xff, 0xff, 0xff, 0x9f, 0x3f, 0x3f, 0xff, 0xf8, 0xe0, 0x83, 0x81, 0x0
};

static PROGMEM WEAK_CONST unsigned char FRAME_ATTACK_1[] =
{
  5, 11, 27, 13,
  0x0, 0x0, 0x8c, 0xc6, 0xee, 0xf6, 0xfe, 0xfe, 0xfe, 0xff, 0xff, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7e, 0xfc, 0xfc, 0xf0, 0xc0, 0x80, 0x80, 0x80, 0x0, 0x0, 0xf, 0x1f, 0x1f, 0x7, 0x3, 0x13, 0x1d, 0x1f, 0x1f, 0x17, 0x7, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1, 0x7, 0xf, 0x13, 0x3, 0x7, 0x1f
};

static PROGMEM WEAK_CONST unsigned char FRAME_ATTACK_2[] =
{
  5, 11, 18, 13,
  0x0, 0x10, 0x38, 0xf8, 0xfc, 0xfe, 0xfe, 0xfe, 0xfe, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xf7, 0x9f, 0x80, 0x0, 0x1c, 0x1e, 0x1f, 0x17, 0x7, 0x3, 0x13, 0x1f, 0x1f, 0x17, 0x11, 0x1, 0x3, 0x3, 0x7, 0x5, 0x1, 0x1
};

static PgmPtr FRAMES_ATTACK[] =
{
  FRAME_ATTACK_0,
  FRAME_ATTACK_1,
  FRAME_ATTACK_2,
};

static PROGMEM WEAK_CONST unsigned char FRAME_STATPONC_0[] =
{
  1, 8, 23, 16,
  0x78, 0xf8, 0xfc, 0xfc, 0xfe, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0x7b, 0x7f, 0xf0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0x80, 0x0, 0x1, 0x0, 0x1, 0x9f, 0xff, 0xff, 0xff, 0x7f, 0xf, 0xff, 0xff, 0xff, 0xf0, 0x80, 0x0, 0x1, 0x1, 0x7, 0x5, 0x1, 0x1, 0x3
};

static PROGMEM WEAK_CONST unsigned char FRAME_STATPONC_1[] =
{
  3, 0, 18, 24,
  0x0, 0x0, 0xe0, 0xfc, 0x1e, 0xf, 0xc4, 0xcc, 0xc0, 0x0, 0x0, 0x0, 0x0, 0x80, 0x80, 0x80, 0x80, 0x0, 0x0, 0xf8, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xfe, 0x7f, 0x1f, 0x7, 0x7, 0x3, 0xf, 0x0, 0x1, 0xc, 0x3f, 0xff, 0xff, 0xff, 0x9f, 0x3f, 0x3f, 0xff, 0xff, 0xfc, 0xe0, 0x80, 0x0, 0x0, 0x0, 0x0, 0x0
};

static PgmPtr FRAMES_STATPONC[] =
{
  FRAME_STATPONC_0,
  FRAME_STATPONC_1,
};

static PROGMEM WEAK_CONST unsigned char FRAME_STATICBASE_0[] =
{
  1, 8, 19, 16,
  0x0, 0xe0, 0xf0, 0xf8, 0x7c, 0xfc, 0xfe, 0xfe, 0xfe, 0xff, 0xff, 0xfe, 0xfe, 0xfc, 0xec, 0xfc, 0xc0, 0x80, 0x0, 0x8, 0xf, 0x7, 0x1f, 0x80, 0xff, 0xff, 0xff, 0x7f, 0x1f, 0xcf, 0xff, 0xff, 0xff, 0xb9, 0x1, 0x33, 0x1f, 0x7f
};

static PROGMEM WEAK_CONST unsigned char FRAME_STATICBASE_1[] =
{
  1, 9, 19, 15,
  0x0, 0xe0, 0xf0, 0xf8, 0x3c, 0xfe, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xfe, 0xfa, 0xfe, 0x80, 0x0, 0x8, 0xf, 0x7, 0xf, 0x46, 0x7f, 0x7f, 0x7f, 0x3f, 0xf, 0x6f, 0x7f, 0x7f, 0x7f, 0x58, 0x1, 0x1b, 0x3f, 0x1f
};

static PgmPtr FRAMES_STATICBASE[] =
{
  FRAME_STATICBASE_0,
  FRAME_STATICBASE_1,
};

const Animation CH_BEAR_ANIMATIONS[] = 
{
  AnimationMake(FRAMES_DEATH, 3, 32, 24),
  AnimationMake(FRAMES_WALK, 4, 32, 24, true),
  AnimationMake(FRAMES_ATTACK, 3, 32, 24),
  AnimationMake(FRAMES_STATPONC, 2, 32, 24),
  AnimationMake(FRAMES_STATICBASE, 2, 32, 24, true),
};

#include <avr/pgmspace.h>

#include "player_animations.h"

static PROGMEM WEAK_CONST unsigned char FRAME_DEATH_0[] =
{
  1, 0, 7, 8,
  0x8, 0x48, 0x67, 0x3f, 0x1f, 0xfc, 0x8
};

static PROGMEM WEAK_CONST unsigned char FRAME_DEATH_1[] =
{
  0, 1, 8, 7,
  0x30, 0x30, 0x3e, 0x1d, 0x1c, 0x7e, 0xe, 0x39
};

static PROGMEM WEAK_CONST unsigned char FRAME_DEATH_2[] =
{
  0, 1, 8, 7,
  0x60, 0x60, 0x7e, 0x78, 0x78, 0x3f, 0x1c, 0x2
};

static PROGMEM WEAK_CONST unsigned char FRAME_DEATH_3[] =
{
  0, 3, 8, 5,
  0x7, 0x7, 0xe, 0x1e, 0x1c, 0xe, 0x16, 0x14
};

static PROGMEM WEAK_CONST unsigned char FRAME_DEATH_4[] =
{
  0, 4, 8, 4,
  0xc, 0xc, 0xe, 0xf, 0xd, 0x4, 0x2, 0x2
};

static PROGMEM WEAK_CONST unsigned char FRAME_DEATH_5[] =
{
  0, 5, 8, 3,
  0x4, 0x6, 0x7, 0x7, 0x7, 0x7, 0x6, 0x4
};

static PgmPtr FRAMES_DEATH[] =
{
  FRAME_DEATH_0,
  FRAME_DEATH_1,
  FRAME_DEATH_2,
  FRAME_DEATH_3,
  FRAME_DEATH_4,
  FRAME_DEATH_5,
};

static PROGMEM WEAK_CONST unsigned char FRAME_IMPACT_BOTTOM_0[] =
{
  1, 0, 7, 7,
  0x1b, 0x7, 0x7f, 0x1f, 0x1f, 0x72, 0x4
};

static PgmPtr FRAMES_IMPACT_BOTTOM[] =
{
  FRAME_IMPACT_BOTTOM_0,
};

static PROGMEM WEAK_CONST unsigned char FRAME_IMPACT_TOP_0[] =
{
  0, 3, 8, 5,
  0x1, 0x11, 0x11, 0x1f, 0xf, 0xf, 0x1b, 0x13
};

static PgmPtr FRAMES_IMPACT_TOP[] =
{
  FRAME_IMPACT_TOP_0,
};

static PROGMEM WEAK_CONST unsigned char FRAME_IMPACT_BACK_0[] =
{
  2, 0, 6, 8,
  0xc2, 0xa2, 0x3e, 0xde, 0xbf, 0x7
};

static PgmPtr FRAMES_IMPACT_BACK[] =
{
  FRAME_IMPACT_BACK_0,
};

static PROGMEM WEAK_CONST unsigned char FRAME_IMPACT_FRONT_0[] =
{
  0, 0, 8, 8,
  0x8, 0x6, 0xff, 0x9f, 0x1f, 0x3c, 0xc4, 0x4
};

static PgmPtr FRAMES_IMPACT_FRONT[] =
{
  FRAME_IMPACT_FRONT_0,
};

static PROGMEM WEAK_CONST unsigned char FRAME_JUMPKICK_0[] =
{
  1, 0, 6, 8,
  0xe0, 0x28, 0x3f, 0x7f, 0x1c, 0x8
};

static PROGMEM WEAK_CONST unsigned char FRAME_JUMPKICK_1[] =
{
  0, 0, 10, 9,
  0x0, 0x0, 0xc, 0xe4, 0x3c, 0x3e, 0x3e, 0x26, 0x21, 0x21, 0x1, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0
};

static PgmPtr FRAMES_JUMPKICK[] =
{
  FRAME_JUMPKICK_0,
  FRAME_JUMPKICK_1,
};

static PROGMEM WEAK_CONST unsigned char FRAME_PUNCH_0[] =
{
  0, 0, 6, 8,
  0x4, 0xe, 0xff, 0xbf, 0x28, 0xc0
};

static PROGMEM WEAK_CONST unsigned char FRAME_PUNCH_1[] =
{
  1, 0, 7, 8,
  0x8, 0xfe, 0x9e, 0x3f, 0xc7, 0x82, 0x2
};

static PgmPtr FRAMES_PUNCH[] =
{
  FRAME_PUNCH_0,
  FRAME_PUNCH_1,
};

static PROGMEM WEAK_CONST unsigned char FRAME_CROUCH_0[] =
{
  1, 1, 7, 7,
  0x42, 0x42, 0x7e, 0x7e, 0x3f, 0x4f, 0x30
};

static PROGMEM WEAK_CONST unsigned char FRAME_CROUCH_1[] =
{
  0, 3, 8, 5,
  0x12, 0x1a, 0x1e, 0x1e, 0x1e, 0x1f, 0x7, 0x18
};

static PgmPtr FRAMES_CROUCH[] =
{
  FRAME_CROUCH_0,
  FRAME_CROUCH_1,
};

static PROGMEM WEAK_CONST unsigned char FRAME_RECEP_0[] =
{
  1, 2, 6, 6,
  0xc, 0x22, 0x3e, 0xe, 0x3f, 0xf
};

static PgmPtr FRAMES_RECEP[] =
{
  FRAME_RECEP_0,
};

static PROGMEM WEAK_CONST unsigned char FRAME_FALL_0[] =
{
  0, 0, 8, 8,
  0x1, 0x1, 0xf, 0xfe, 0x1f, 0x57, 0x22, 0x2
};

static PROGMEM WEAK_CONST unsigned char FRAME_FALL_1[] =
{
  0, 0, 7, 8,
  0x2, 0x2, 0xe, 0xfe, 0x1e, 0x57, 0x21
};

static PgmPtr FRAMES_FALL[] =
{
  FRAME_FALL_0,
  FRAME_FALL_1,
};

static PROGMEM WEAK_CONST unsigned char FRAME_JUMP_0[] =
{
  0, 2, 8, 6,
  0x1, 0x1, 0x39, 0x2f, 0xe, 0x3e, 0x2f, 0x1
};

static PROGMEM WEAK_CONST unsigned char FRAME_JUMP_1[] =
{
  0, 0, 8, 8,
  0x8, 0x4, 0x3e, 0xde, 0x1f, 0x33, 0x4, 0x4
};

static PgmPtr FRAMES_JUMP[] =
{
  FRAME_JUMP_0,
  FRAME_JUMP_1,
};

static PROGMEM WEAK_CONST unsigned char FRAME_RUNSTOP_0[] =
{
  0, 0, 8, 8,
  0x4, 0x4, 0x7e, 0x9e, 0x3f, 0x6b, 0xc0, 0x80
};

static PgmPtr FRAMES_RUNSTOP[] =
{
  FRAME_RUNSTOP_0,
};

static PROGMEM WEAK_CONST unsigned char FRAME_RUN_0[] =
{
  1, 1, 7, 7,
  0x18, 0x4, 0x7c, 0x2e, 0x3e, 0xf, 0x7
};

static PROGMEM WEAK_CONST unsigned char FRAME_RUN_1[] =
{
  0, 1, 8, 7,
  0x4, 0x2, 0x62, 0x3e, 0xf, 0xf, 0x17, 0x63
};

static PROGMEM WEAK_CONST unsigned char FRAME_RUN_2[] =
{
  2, 1, 5, 7,
  0x28, 0x3e, 0x7e, 0xf, 0x7
};

static PROGMEM WEAK_CONST unsigned char FRAME_RUN_3[] =
{
  1, 1, 6, 7,
  0xc, 0x42, 0x7e, 0x1f, 0x1f, 0x6b
};

static PgmPtr FRAMES_RUN[] =
{
  FRAME_RUN_0,
  FRAME_RUN_1,
  FRAME_RUN_2,
  FRAME_RUN_3,
};

static PROGMEM WEAK_CONST unsigned char FRAME_WALK_0[] =
{
  1, 0, 6, 8,
  0x4, 0xc4, 0x7e, 0xfe, 0x1f, 0x17
};

static PROGMEM WEAK_CONST unsigned char FRAME_WALK_1[] =
{
  1, 1, 6, 7,
  0x8, 0x6c, 0x5e, 0x1e, 0x6f, 0x47
};

static PROGMEM WEAK_CONST unsigned char FRAME_WALK_2[] =
{
  0, 0, 7, 8,
  0x4, 0x2, 0x22, 0xfe, 0x9e, 0x3f, 0x27
};

static PROGMEM WEAK_CONST unsigned char FRAME_WALK_3[] =
{
  2, 1, 5, 7,
  0x74, 0x5e, 0x1e, 0x6f, 0x4f
};

static PgmPtr FRAMES_WALK[] =
{
  FRAME_WALK_0,
  FRAME_WALK_1,
  FRAME_WALK_2,
  FRAME_WALK_3,
};

static PROGMEM WEAK_CONST unsigned char FRAME_STAT_0[] =
{
  1, 0, 6, 8,
  0x4, 0xe2, 0xbe, 0x1e, 0xff, 0xa7
};

static PgmPtr FRAMES_STAT[] =
{
  FRAME_STAT_0,
};

const Animation PLAYER_ANIMATIONS[] = 
{
  AnimationMake(FRAMES_DEATH, 6),
  AnimationMake(FRAMES_IMPACT_BOTTOM, 1),
  AnimationMake(FRAMES_IMPACT_TOP, 1),
  AnimationMake(FRAMES_IMPACT_BACK, 1),
  AnimationMake(FRAMES_IMPACT_FRONT, 1),
  AnimationMake(FRAMES_JUMPKICK, 2),
  AnimationMake(FRAMES_PUNCH, 2),
  AnimationMake(FRAMES_CROUCH, 2),
  AnimationMake(FRAMES_RECEP, 1),
  AnimationMake(FRAMES_FALL, 2),
  AnimationMake(FRAMES_JUMP, 2),
  AnimationMake(FRAMES_RUNSTOP, 1),
  AnimationMake(FRAMES_RUN, 4),
  AnimationMake(FRAMES_WALK, 4),
  AnimationMake(FRAMES_STAT, 1),
};

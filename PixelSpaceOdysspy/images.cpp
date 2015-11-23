#include <avr/pgmspace.h>
#include "images.h"

PROGMEM static const unsigned char death_0[] =
{
  4, 7, 8, 9,
  0x10, 0x90, 0xcb, 0x7f, 0x3e, 0xf8, 0x10, 0x20, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1, 0x0, 0x0
};

PROGMEM static const unsigned char death_1[] =
{
  3, 9, 9, 7,
  0x30, 0x30, 0x3e, 0x1d, 0x1c, 0x7e, 0xe, 0x39, 0x1
};

PROGMEM static const unsigned char death_2[] =
{
  3, 9, 9, 7,
  0x60, 0x60, 0x7e, 0x78, 0x78, 0x3f, 0x1c, 0x2, 0x2
};

PROGMEM static const unsigned char death_3[] =
{
  3, 11, 9, 5,
  0x7, 0x7, 0xe, 0x1e, 0x1c, 0x1c, 0xe, 0x16, 0x14
};

PROGMEM static const unsigned char death_4[] =
{
  2, 12, 11, 4,
  0xc, 0xc, 0xc, 0xe, 0xf, 0xd, 0xd, 0xc, 0x4, 0x2, 0x2
};

PROGMEM static const unsigned char death_5[] =
{
  2, 13, 11, 3,
  0x4, 0x4, 0x6, 0x7, 0x7, 0x7, 0x7, 0x7, 0x6, 0x4, 0x4
};

static const FrameData death_frames[] =
{
  death_0,
  death_1,
  death_2,
  death_3,
  death_4,
  death_5,
};

PROGMEM static const unsigned char impact_bottom_0[] =
{
  4, 6, 7, 8,
  0x18, 0x4, 0x7e, 0x1e, 0x3f, 0xcf, 0x13
};

PROGMEM static const unsigned char impact_bottom_1[] =
{
  4, 7, 7, 9,
  0x4, 0xc2, 0x3e, 0x3e, 0xfe, 0x17, 0x13, 0x0, 0x1, 0x0, 0x0, 0x0, 0x1, 0x0
};

static const FrameData impact_bottom_frames[] =
{
  impact_bottom_0,
  impact_bottom_1,
};

PROGMEM static const unsigned char impact_top_0[] =
{
  3, 11, 8, 5,
  0x1, 0x11, 0x11, 0x1f, 0xf, 0xf, 0x1b, 0x13
};

PROGMEM static const unsigned char impact_top_1[] =
{
  3, 9, 7, 7,
  0x4, 0x4, 0x62, 0x3e, 0xe, 0x7f, 0x57
};

static const FrameData impact_top_frames[] =
{
  impact_top_0,
  impact_top_1,
};

PROGMEM static const unsigned char impact_back_0[] =
{
  5, 8, 8, 8,
  0xc0, 0xa2, 0x32, 0xde, 0xbe, 0xe, 0x7, 0x7
};

PROGMEM static const unsigned char impact_back_1[] =
{
  5, 8, 7, 8,
  0xc0, 0xac, 0x3e, 0xfe, 0xbe, 0x27, 0x7
};

static const FrameData impact_back_frames[] =
{
  impact_back_0,
  impact_back_1,
};

PROGMEM static const unsigned char impact_front_0[] =
{
  3, 7, 7, 9,
  0x33, 0xf, 0xff, 0x3c, 0x38, 0xc8, 0x88, 0x0, 0x0, 0x1, 0x1, 0x0, 0x0, 0x0
};

PROGMEM static const unsigned char impact_front_1[] =
{
  2, 7, 9, 9,
  0x8, 0x8, 0x4, 0xfc, 0x3f, 0x3f, 0x78, 0x88, 0x8, 0x0, 0x0, 0x0, 0x1, 0x1, 0x0, 0x0, 0x1, 0x0
};

static const FrameData impact_front_frames[] =
{
  impact_front_0,
  impact_front_1,
};

PROGMEM static const unsigned char kick_0[] =
{
  4, 8, 6, 8,
  0xe0, 0x28, 0x3f, 0x7f, 0x1c, 0x8
};

PROGMEM static const unsigned char kick_1[] =
{
  5, 9, 8, 7,
  0x6, 0x72, 0x1e, 0x1f, 0x1f, 0x13, 0x10, 0x10
};

static const FrameData kick_frames[] =
{
  kick_0,
  kick_1,
};

PROGMEM static const unsigned char punch_0[] =
{
  3, 8, 7, 8,
  0x4, 0x2, 0xee, 0xbf, 0x3f, 0xc8, 0x80
};

PROGMEM static const unsigned char punch_1[] =
{
  5, 8, 10, 8,
  0xc0, 0xb0, 0x18, 0xfe, 0x8e, 0xf, 0x7, 0x2, 0x2, 0x2
};

PROGMEM static const unsigned char punch_2[] =
{
  5, 8, 6, 8,
  0xc0, 0xbc, 0x3e, 0xfe, 0x9f, 0x17
};

static const FrameData punch_frames[] =
{
  punch_0,
  punch_1,
  punch_2,
};

PROGMEM static const unsigned char crouch_0[] =
{
  4, 9, 7, 7,
  0x42, 0x62, 0x7e, 0x1e, 0x7f, 0x57, 0x20
};

PROGMEM static const unsigned char crouch_1[] =
{
  4, 11, 8, 5,
  0x12, 0x1a, 0x1e, 0x1e, 0x1e, 0x1f, 0x7, 0x18
};

static const FrameData crouch_frames[] =
{
  crouch_0,
  crouch_1,
};

PROGMEM static const unsigned char recep_0[] =
{
  3, 10, 9, 6,
  0x4, 0x2, 0x22, 0x3f, 0x7, 0x37, 0x1b, 0x3, 0x4
};

PROGMEM static const unsigned char recep_1[] =
{
  4, 10, 8, 6,
  0xc, 0x22, 0x3e, 0xe, 0x3e, 0x3f, 0xf, 0x10
};

PROGMEM static const unsigned char recep_2[] =
{
  4, 9, 7, 7,
  0x18, 0x66, 0x5e, 0x1e, 0x7e, 0x47, 0x7
};

static const FrameData recep_frames[] =
{
  recep_0,
  recep_1,
  recep_2,
};

PROGMEM static const unsigned char fall_0[] =
{
  4, 7, 8, 9,
  0x1, 0x1, 0xf, 0xfe, 0x1f, 0xd7, 0x22, 0x2, 0x0, 0x0, 0x0, 0x1, 0x0, 0x0, 0x0, 0x0
};

PROGMEM static const unsigned char fall_1[] =
{
  4, 7, 8, 9,
  0x2, 0x2, 0xe, 0xfe, 0x1f, 0xd7, 0x21, 0x1, 0x0, 0x0, 0x0, 0x1, 0x0, 0x0, 0x0, 0x0
};

static const FrameData fall_frames[] =
{
  fall_0,
  fall_1,
};

PROGMEM static const unsigned char jump_0[] =
{
  3, 10, 9, 6,
  0x1, 0x1, 0x39, 0x2f, 0xe, 0x3e, 0x2e, 0xf, 0x1
};

PROGMEM static const unsigned char jump_1[] =
{
  4, 8, 7, 8,
  0x18, 0xc4, 0x3e, 0xe, 0xfe, 0xf, 0x37
};

PROGMEM static const unsigned char jump_2[] =
{
  3, 9, 9, 7,
  0x20, 0x72, 0x12, 0x1e, 0x1e, 0x1f, 0x17, 0x72, 0x2
};

PROGMEM static const unsigned char jump_3[] =
{
  4, 7, 8, 9,
  0x2, 0x2, 0x3e, 0xde, 0x1f, 0x33, 0x2, 0x2, 0x0, 0x0, 0x0, 0x0, 0x1, 0x0, 0x0, 0x0
};

static const FrameData jump_frames[] =
{
  jump_0,
  jump_1,
  jump_2,
  jump_3,
};

PROGMEM static const unsigned char run_stop_0[] =
{
  3, 8, 8, 8,
  0x4, 0x4, 0x7e, 0x9e, 0x3f, 0x6b, 0xc0, 0x80
};

static const FrameData run_stop_frames[] =
{
  run_stop_0,
};

PROGMEM static const unsigned char run_0[] =
{
  4, 8, 7, 8,
  0x18, 0x4, 0xfc, 0x5e, 0x7e, 0xf, 0x7
};

PROGMEM static const unsigned char run_1[] =
{
  3, 7, 8, 8,
  0x8, 0x4, 0xc4, 0x7c, 0x1e, 0x1e, 0x2f, 0xc7
};

PROGMEM static const unsigned char run_2[] =
{
  5, 8, 5, 8,
  0x58, 0x7e, 0xfe, 0x1f, 0x7
};

PROGMEM static const unsigned char run_3[] =
{
  4, 7, 6, 8,
  0x18, 0x84, 0xfc, 0x7e, 0x7f, 0x97
};

static const FrameData run_frames[] =
{
  run_0,
  run_1,
  run_2,
  run_3,
};

PROGMEM static const unsigned char walk_0[] =
{
  3, 8, 7, 8,
  0x4, 0x2, 0x22, 0xfe, 0x9e, 0x3f, 0x27
};

PROGMEM static const unsigned char walk_1[] =
{
  4, 9, 6, 7,
  0x8, 0x6c, 0x5e, 0x1e, 0x6f, 0x47
};

PROGMEM static const unsigned char walk_2[] =
{
  4, 8, 7, 8,
  0x4, 0xc4, 0x7e, 0xfe, 0x3f, 0x27, 0x30
};

PROGMEM static const unsigned char walk_3[] =
{
  5, 9, 5, 7,
  0x74, 0x5e, 0x1e, 0x6f, 0x4f
};

static const FrameData walk_frames[] =
{
  walk_0,
  walk_1,
  walk_2,
  walk_3,
};

PROGMEM static const unsigned char stat_1_0[] =
{
  4, 8, 6, 8,
  0x4, 0xe2, 0xbe, 0x1e, 0xff, 0xa7
};

PROGMEM static const unsigned char stat_1_1[] =
{
  4, 8, 6, 8,
  0x2, 0xe1, 0x9f, 0xf, 0xff, 0x97
};

static const FrameData stat_1_frames[] =
{
  stat_1_0,
  stat_1_1,
};

PROGMEM static const unsigned char stat_2_0[] =
{
  3, 7, 7, 9,
  0x10, 0x8, 0xfc, 0x3c, 0x7f, 0xc7, 0x0, 0x0, 0x0, 0x1, 0x1, 0x0, 0x1, 0x1
};

static const FrameData stat_2_frames[] =
{
  stat_2_0,
};

PROGMEM static const unsigned char stat_3_0[] =
{
  4, 7, 6, 9,
  0x10, 0xfc, 0x3f, 0x3f, 0xfc, 0x30, 0x0, 0x1, 0x1, 0x0, 0x1, 0x0
};

PROGMEM static const unsigned char stat_3_1[] =
{
  5, 8, 6, 8,
  0xc7, 0xbf, 0x1e, 0xfe, 0xc, 0x30
};

static const FrameData stat_3_frames[] =
{
  stat_3_0,
  stat_3_1,
};

const Animation animations[] = 
{
  CreateAnimation(death_frames, 6),
  CreateAnimation(impact_bottom_frames, 2),
  CreateAnimation(impact_top_frames, 2),
  CreateAnimation(impact_back_frames, 2),
  CreateAnimation(impact_front_frames, 2),
  CreateAnimation(kick_frames, 2),
  CreateAnimation(punch_frames, 3),
  CreateAnimation(crouch_frames, 2),
  CreateAnimation(recep_frames, 3),
  CreateAnimation(fall_frames, 2),
  CreateAnimation(jump_frames, 4),
  CreateAnimation(run_stop_frames, 1),
  CreateAnimation(run_frames, 4),
  CreateAnimation(walk_frames, 4),
  CreateAnimation(stat_1_frames, 2),
  CreateAnimation(stat_2_frames, 1),
  CreateAnimation(stat_3_frames, 2),
};
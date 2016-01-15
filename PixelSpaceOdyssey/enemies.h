#ifndef ENEMIES
#define ENEMIES

#include "character.h"

#define ENEMY_TYPE_SPIDER_LARGE 0
#define ENEMY_TYPE_SPIDER_SMALL 1
#define ENEMY_TYPE_BEAR         2
#define ENEMY_TYPE_DOG          3
#define ENEMY_TYPE_COUNT        4

typedef uint16_t EnemyType;

#include "enemy_base.h"

#include "spider_large.h"
#include "dog.h"
#include "bear.h"

#endif // ENEMIES


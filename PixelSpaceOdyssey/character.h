//
//  character.h
//  PixelSpaceOdessey
//
//  Created by Alex Lementuev on 11/26/15.
//  Copyright © 2015 Space Madness. All rights reserved.
//

#ifndef character_h
#define character_h

#include <string.h>

#include "animation.h"

#define DIR_LEFT -1
#define DIR_RIGHT 1

#define CHARACTER_CALLBACK_ANIMATION_FINISHED 1
#define CHARACTER_CALLBACK_OBSTACLE_WALL 2
#define CHARACTER_CALLBACK_OBSTACLE_TRENCH 3

typedef int8_t Direction;
typedef uint8_t CharacterCallbackType;

struct _Character;
typedef struct _Character Character;

typedef void (*CharacterInit)(Character *character);
typedef void (*CharacterCallback)(Character *character, CharacterCallbackType type, int16_t user1, int16_t user2);
typedef void (*CharacterBehaviour)(Character *character, TimeInterval dt);

typedef struct _Character
{
    CharacterBehaviour behaviour;
    CharacterCallback callback;
    const Animation* animation;

    uint16_t user1;
    uint16_t user2;

    uint16_t width;
    uint16_t height;
    uint16_t colliderWidth;
    uint16_t colliderHeight;
    int16_t x;
    int16_t y;
    uint16_t frameTime;
    Direction dir;
    int8_t move;
    uint8_t frame;
    boolean animationEnded;
} Character;

inline Character CharacterMake(uint16_t width, uint16_t height)
{
    Character character;
    memset(&character, 0, sizeof(character));
    character.width = width;
    character.height = height;
    return character;
}

void CharacterSetAnimation(Character* character, const Animation* animation);
void CharacterUpdate(Character* character, TimeInterval dt);
void CharacterDraw(Character* character);
void CharacterCallbackInvoke(Character *character, CharacterCallbackType type, int16_t user1 = 0, int16_t user2 = 0);

#endif /* character_h */

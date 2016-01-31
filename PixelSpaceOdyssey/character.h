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

#include "common.h"
#include "animation.h"
#include "tilemap.h"

#define DIR_LEFT -1
#define DIR_NONE  0
#define DIR_RIGHT 1

#define CHARACTER_CALLBACK_ANIMATION_FINISHED 1
#define CHARACTER_CALLBACK_OBSTACLE 2
#define CHARACTER_CALLBACK_REACHED_TARGET 3

typedef int8_t Direction;
typedef uint8_t CharacterCallbackType;

struct _Character;
typedef struct _Character Character;

typedef void (*CharacterInit)(Character *character);
typedef void (*CharacterCallback)(Character *character, CharacterCallbackType type, int16_t user1, int16_t user2);
typedef void (*CharacterBehaviour)(Character *character, TimeInterval dt);

enum CharacterInitialState {
    CharacterInitialStateUndefined,
    CharacterInitialStateStat,          // stay still until it see the player
    CharacterInitialStateSleep,         // sleep until player wake is up
    CharacterInitialStatePatrol,        // patroling the area
};

enum CharacterPatrollingType {
    CharacterPatrollingTypeUndefined,
    CharacterPatrollingTypeNone,        // stay in the last position
    CharacterPatrollingTypeForever,     // once started partoling - never stops
    CharacterPatrollingTypeReturnToBase // returns to initial position after time out
};

typedef struct _Character
{
    CharacterBehaviour behaviour;
    CharacterCallback callback;

    const Animation *animations;
    const uint8_t *animationLookup;
    const Animation *animation;

    TimeInterval lastAttackTimestamp;
    
    uint16_t state;
    uint16_t stateTime;
    uint16_t flags;

    uint16_t width;
    uint16_t height;
    uint16_t sightDistanceForward;
    uint16_t sightDistanceBackward;
    int16_t x;
    int16_t y;
    int16_t lastPlayerX;
    int16_t targetPos; // a target pos for a non-player character
    int16_t basePos;   // a 'base' pos for a non-player character (spawn point)
    int16_t moveMinX;
    int16_t moveMaxX;
    int16_t sightMinX;
    int16_t sightMaxX;
    uint16_t frameTime;
    Direction dir;
    uint8_t move;
    uint8_t moveMax;
    uint8_t frame;
    boolean animationEnded;
    boolean canSeePlayer;
    boolean hasTarget;

    CharacterInitialState initialState;
    CharacterPatrollingType patrollingType;
    
    #if EMULATOR
    uint16_t id;
    #endif
    
} Character;

inline bool CharacterIsMoving(const Character *character)    { return character->move != 0; }

inline int16_t CharacterGetTop(const Character *character)   { return character->y - DIV2(character->height); }
inline int16_t CharacterGetBottom(const Character *character){ return character->y + DIV2(character->height); }
inline int16_t CharacterGetLeft(const Character *character)  { return character->x - DIV2(character->width); }
inline int16_t CharacterGetRight(const Character *character) { return character->x + DIV2(character->width); }
inline int16_t CharacterGetRow(const Character *character)   { return TILE_GET_ROW(character->y); }
inline int16_t CharacterGetCol(const Character *character)   { return TILE_GET_COL(character->x); }

inline void CharacterSetTop(Character *character, int16_t y)    { character->y = y + DIV2(character->height); }
inline void CharacterSetBottom(Character *character, int16_t y) { character->y = y - DIV2(character->height); }
inline void CharacterSetLeft(Character *character, int16_t x)   { character->x = x + DIV2(character->width); }
inline void CharacterSetRight(Character *character, int16_t x)  { character->x = x - DIV2(character->width); }

inline void CharacterSetUserFlag(Character *character, uint8_t mask, bool flag)
{
    if (flag)
    {
        character->flags |= mask;
    }
    else
    {
        character->flags &= ~mask;
    }
}

inline bool CharacterHasUserFlag(const Character *character, uint8_t mask)
{
    return character->flags & mask;
}

inline void CharacterToggleUserFlag(Character *character, uint8_t mask)
{
    bool flag = CharacterHasUserFlag(character, mask);
    CharacterSetUserFlag(character, mask, !flag);
}

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

#include "enemy_base.h"

#include "game.h"

inline int16_t CharacterGetTop(const Character *character)   { return character->y - DIV2(character->colliderHeight); }
inline int16_t CharacterGetBottom(const Character *character){ return character->y + DIV2(character->colliderHeight); }
inline int16_t CharacterGetLeft(const Character *character)  { return character->x - DIV2(character->colliderWidth); }
inline int16_t CharacterGetRight(const Character *character) { return character->x + DIV2(character->colliderWidth); }

inline void CharacterSetTop(Character *character, int16_t y)    { character->y = y + DIV2(character->colliderHeight); }
inline void CharacterSetBottom(Character *character, int16_t y) { character->y = y - DIV2(character->colliderHeight); }
inline void CharacterSetLeft(Character *character, int16_t x)   { character->x = x + DIV2(character->colliderWidth); }
inline void CharacterSetRight(Character *character, int16_t x)  { character->x = x - DIV2(character->colliderWidth); }

inline static void handleTileHorCollision(Character *character, const Tile& tile)
{
    if (character->x > tile.x) // tile on the left
    {
        CharacterSetLeft(character, TILE_GET_RIGHT(tile));
    }
    else
    {
        CharacterSetRight(character, TILE_GET_LEFT(tile));
    }
    
    CharacterCallbackInvoke(character, CHARACTER_CALLBACK_OBSTACLE_WALL);
}

inline static void handleTrench(Character *character, int16_t x, int16_t bottom)
{
    Tile tile;
    if (character->dir == DIR_LEFT)
    {
        getSolidTile(x + TILE_WIDTH, bottom, &tile);
        CharacterSetLeft(character, TILE_GET_LEFT(tile));
    }
    else if (character->dir == DIR_RIGHT)
    {
        getSolidTile(x - TILE_WIDTH, bottom, &tile);
        CharacterSetRight(character, TILE_GET_RIGHT(tile));
    }
    
    CharacterCallbackInvoke(character, CHARACTER_CALLBACK_OBSTACLE_TRENCH);
}

void EnemyUpdate(Character* character, TimeInterval dt)
{
    // update AI
    CharacterBehaviour behaviour = character->behaviour;
    if (behaviour)
    {
        behaviour(character, dt);
    }
    
    // update movement
    
    int8_t move = character->move;
    if (move != 0)
    {
        character->x += character->dir * character->move * WALK_SPEED;
        
        // handle collisions (only horizontal)
        Tile tile;
        
        int16_t left = CharacterGetLeft(character);
        int16_t right = CharacterGetRight(character);
        int16_t bottom = CharacterGetBottom(character);
        
        if (getSolidTile(left, character->y, &tile) ||
            getSolidTile(right, character->y, &tile))
        {
            handleTileHorCollision(character, tile);
        }
        else if (character->dir == DIR_LEFT && !getSolidTile(left, bottom, &tile))
        {
            handleTrench(character, left, bottom);
        }
        else if (character->dir == DIR_RIGHT && !getSolidTile(right, bottom, &tile))
        {
            handleTrench(character, right, bottom);
        }
    }
    
    // update character
    CharacterUpdate(character, dt);
}
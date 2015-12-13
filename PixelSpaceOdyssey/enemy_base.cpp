#include "enemy_base.h"

#include "game.h"

void EnemyUpdate(Character* character, TimeInterval dt)
{
    // update AI
    CharacterBehaviour behaviour = character->behaviour;
    if (behaviour)
    {
        behaviour(character, dt);
    }
    
    // update movement
    character->x += character->dir * character->move * WALK_SPEED;
    
    // handle collisions
    // TODO
    
    // update character
    CharacterUpdate(character, dt);
}
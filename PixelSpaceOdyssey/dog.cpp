#include "dog.h"

#include "animation.h"
#include "dog_animations.h"

#include "game.h"

#define DogStateAttack  DOG_ANIMATION_ATTACK
#define DogStateRun     DOG_ANIMATION_RUN
#define DogStateStat    DOG_ANIMATION_STAT

typedef uint16_t DogState;

static inline void setAnimation(Character *character, int index)
{
    assert(index >= 0 && index < DOG_ANIMATIONS_COUNT);
    CharacterSetAnimation(character, &DOG_ANIMATIONS[index]);
}

static inline DogState getState(Character *character)
{
    return (DogState) character->user1;
}

static inline void setState(Character *character, DogState state)
{
    character->user1 = (uint16_t) state;
    setAnimation(character, state);
}

static inline void run(Character *character)
{
    setState(character, DogStateRun);
    character->move = 3;
}

static inline void attack(Character *character)
{
    setState(character, DogStateAttack);
}

void EnemyInitDog(Character *character)
{
    setState(character, DogStateStat);
}

void EnemyCallbackDog(Character *character, CharacterCallbackType type, int16_t user1, int16_t user2)
{
    switch (type)
    {
        case CHARACTER_CALLBACK_ANIMATION_FINISHED:
        {
            DogState state = getState(character);
            switch (state)
            {
                case DogStateAttack:
                    run(character);
                    break;
            }
            break;
        }
    }
}

void EnemyBehaviourDog(Character *character, TimeInterval dt)
{
    DogState state = getState(character);
    switch (state)
    {
        case DogStateStat:
        {
            uint16_t distanceSqr = playerDistanceSqr(character);
            if (distanceSqr < 2500)
            {
                run(character);
            }
            break;
        }
            
        case DogStateRun:
        {
            uint16_t distanceSqr = playerDistanceSqr(character);
            
            int16_t distance = player.x - character->x;
            character->dir = distance < 0 ? DIR_LEFT : DIR_RIGHT;
            
            if (distanceSqr < 50)
            {
                attack(character);
            }
            else
            {
                character->x += character->dir * character->move * WALK_SPEED;
            }
            break;
        }
    }
}



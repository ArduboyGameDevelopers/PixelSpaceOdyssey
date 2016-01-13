#include "dog.h"

#include "animation.h"
#include "dog_animations.h"

#include "game.h"

#define DogStateStat    0
#define DogStateRun     1
#define DogStateAttack  2

static const uint8_t ANIMATION_LOOKUP[] = {
    DOG_ANIMATION_STAT,     /* DogStateStat */
    DOG_ANIMATION_RUN,      /* DogStateRun */
    DOG_ANIMATION_ATTACK    /* DogStateAttack */
};

typedef uint16_t DogState;

static inline void setAnimation(Character *self, int index)
{
    assert(index >= 0 && index < DOG_ANIMATIONS_COUNT);
    CharacterSetAnimation(self, &DOG_ANIMATIONS[index]);
}

static inline DogState getState(Character *self)
{
    return (DogState) self->state;
}

static inline void setState(Character *self, DogState state)
{
    self->state = (uint16_t) state;
    self->stateTime = 0;
    self->move = 0;
    
    setAnimation(self, ANIMATION_LOOKUP[state]);
}

static inline void run(Character *self)
{
    setState(self, DogStateRun);
    self->move = 3;
}

static inline void stat(Character *self)
{
    setState(self, DogStateStat);
}

static inline void attack(Character *self)
{
    EnemyAttack(self);
    setState(self, DogStateAttack);
}

void EnemyInitDog(Character *character)
{
    setState(character, DogStateStat);
}

void EnemyCallbackDog(Character *self, CharacterCallbackType type, int16_t, int16_t)
{
    switch (type)
    {
        case CHARACTER_CALLBACK_ANIMATION_FINISHED:
        {
            DogState state = getState(self);
            switch (state)
            {
                case DogStateAttack:
                    run(self);
                    break;
            }
            break;
        }
            
        case CHARACTER_CALLBACK_OBSTACLE:
        {
            DogState state = getState(self);
            switch (state)
            {
                case DogStateRun:
                    stat(self);
                    break;
            }
            break;
        }
    }
}

void EnemyBehaviourDog(Character *self, TimeInterval)
{
    DogState state = getState(self);
    switch (state)
    {
        case DogStateStat:
        case DogStateRun:
        {
            EnemyUpdatePlayerPos(self);
            
            int16_t distance = self->lastPlayerX - self->x;
            self->dir = distance < 0 ? DIR_LEFT : DIR_RIGHT;
            
            if (abs(distance) < (self->canSeePlayer ? DIV2(self->colliderWidth + player.width) : 10))
            {
                if (self->canSeePlayer && EnemyCanAttack(self))
                {
                    attack(self);
                }
                else
                {
                    stat(self);
                }
            }
            else if (self->canSeePlayer)
            {
                if (self->move == 0 && EnemyCanMove(self))
                {
                    run(self);
                }
            }
            break;
        }
    }
}



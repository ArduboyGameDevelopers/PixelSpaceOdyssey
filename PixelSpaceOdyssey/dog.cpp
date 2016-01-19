#include "dog.h"

#include "animation.h"
#include "ch_dog_animations.h"

#include "game.h"

#define DogStateStat    0
#define DogStateChase   1
#define DogStateAttack  2

static const uint8_t ANIMATION_LOOKUP[] = {
    CH_DOG_ANIMATION_STAT,     /* DogStateStat */
    CH_DOG_ANIMATION_RUN,      /* DogStateRun */
    CH_DOG_ANIMATION_ATTACK    /* DogStateAttack */
};

typedef uint16_t DogState;

static inline void setAnimation(Character *self, int index)
{
    assert(index >= 0 && index < CH_DOG_ANIMATIONS_COUNT);
    CharacterSetAnimation(self, &CH_DOG_ANIMATIONS[index]);
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

static inline void chase(Character *self)
{
    setState(self, DogStateChase);
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
                    chase(self);
                    break;
            }
            break;
        }
            
        case CHARACTER_CALLBACK_OBSTACLE:
        {
            DogState state = getState(self);
            switch (state)
            {
                case DogStateChase:
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
        case DogStateChase:
        {
            EnemyUpdatePlayerPos(self);
            
            int16_t distance = self->lastPlayerX - self->x;
            self->dir = distance < 0 ? DIR_LEFT : DIR_RIGHT;
            
            if (abs(distance) < (self->canSeePlayer ? DIV2(self->width + player.width) : 10))
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
                    chase(self);
                }
            }
            break;
        }
    }
}



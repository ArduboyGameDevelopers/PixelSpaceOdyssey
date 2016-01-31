#include "bear.h"
#include "enemy_base.h"
#include "game.h"

#define BearStateStat   0
#define BearStatePonc   1
#define BearStateWalk   2
#define BearStateAttack 3
#define BearStatePatrol 4

static const uint8_t ANIMATION_LOOKUP[] = {
    CH_BEAR_ANIMATION_STATICBASE, /* BearStateStat */
    CH_BEAR_ANIMATION_STATPONC,   /* BearStatePonc */
    CH_BEAR_ANIMATION_WALK,       /* BearStateWalk */
    CH_BEAR_ANIMATION_ATTACK,     /* BearStateAttack */
    CH_BEAR_ANIMATION_WALK,       /* BearStatePatrol */
};

typedef uint16_t BearState;

static inline void setAnimation(Character *self, int index)
{
    assert(index >= 0 && index < CH_BEAR_ANIMATIONS_COUNT);
    CharacterSetAnimation(self, &CH_BEAR_ANIMATIONS[index]);
}

static inline BearState getState(Character *self)
{
    return (BearState) self->state;
}

static inline void setState(Character *self, BearState state)
{
    self->state = (uint16_t) state;
    self->stateTime = 0;
    self->move = 0;
    setAnimation(self, ANIMATION_LOOKUP[state]);
}

static inline void ponc(Character *self)
{
    setState(self, BearStatePonc);
}

static inline void walk(Character *self)
{
    setState(self, BearStateWalk);
    self->move = 1;
}

static inline void patrol(Character *self)
{
    setState(self, BearStatePatrol);
    self->move = 1;
}

static inline void stat(Character *self)
{
    setState(self, BearStateStat);
}

static inline void attack(Character *self)
{
    setState(self, BearStateAttack);
    EnemyAttackBaseBase(self);
}

void EnemyInitBear(Character *character)
{
    setState(character, BearStateStat);
}

void EnemyCallbackBear(Character *self, CharacterCallbackType type, int16_t, int16_t)
{
    BearState state = getState(self);
    switch (type)
    {
        case CHARACTER_CALLBACK_ANIMATION_FINISHED:
        {
            switch (state)
            {
                case BearStatePonc:
                case BearStateAttack:
                    walk(self);
                    break;
            }
            break;
        }
            
        case CHARACTER_CALLBACK_OBSTACLE:
        {
            switch (state)
            {
                case BearStateWalk:
                    stat(self);
                    break;
                case BearStatePatrol:
                    self->dir = -self->dir;
                    break;
            }
            break;
        }
    }
}

void EnemyBehaviourBear(Character *self, TimeInterval dt)
{
    BearState state = getState(self);
    
    switch (state)
    {
        case BearStateWalk:
        case BearStateStat:
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
                self->stateTime = 0;
                if (self->move == 0 && EnemyCanMove(self))
                {
                    walk(self);
                }
            }
            else if (self->move == 0)
            {
                self->stateTime += dt;
                if (self->stateTime > 5000)
                {
                    patrol(self);
                }
            }
            break;
        }
            
        case BearStatePatrol:
        {
            EnemyUpdatePlayerPos(self);
            if (self->canSeePlayer)
            {
                walk(self);
            }
            break;
        }
    }
}

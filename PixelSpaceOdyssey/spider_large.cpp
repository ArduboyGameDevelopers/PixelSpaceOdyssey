#include "spider_large.h"

#include "animation.h"
#include "ch_spider_large_animations.h"

#include "game.h"
#include "enemy_base.h"

static const EnemyState EnemyStateSleep   = EnemyStateUser;
static const EnemyState EnemyStateAwaken  = EnemyStateUser + 1;
static const EnemyState EnemyStateRise    = EnemyStateUser + 2;

static inline void awake(Character *self)
{
    EnemySetState(self, EnemyStateAwaken);
}

static inline void rise(Character *self)
{
    EnemySetState(self, EnemyStateRise);
}

void EnemyInitSpiderLarge(Character *self)
{
    if (self->initialState == CharacterInitialStatePatrol)
    {
        EnemyPatrol(self);
    }
    else
    {
        EnemySetState(self, EnemyStateSleep);
    }
}

void EnemyCallbackSpiderLarge(Character *self, CharacterCallbackType type, int16_t user1, int16_t user2)
{
    if (type == CHARACTER_CALLBACK_ANIMATION_FINISHED &&
        EnemyStateRise == EnemyGetState(self))
    {
        EnemyStat(self);
    }
    else
    {
        EnemyDefaultCallback(self, type, user1, user2);
    }
}

void EnemyBehaviourSpiderLarge(Character *self, TimeInterval dt)
{
    EnemyState state = EnemyGetState(self);
    
    switch (state)
    {
        case EnemyStateSleep:
        {
            EnemyUpdatePlayerPos(self);
            if (self->canSeePlayer)
            {
                awake(self);
            }
            break;
        }
        case EnemyStateAwaken:
        {
            self->stateTime += dt;
            if (self->stateTime > 500)
            {
                rise(self);
            }
            break;
        }
        case EnemyStateRise:
        {
            break;
        }
        default:
        {
            EnemyDefaultBehaviour(self, dt);
            break;
        }
    }
}

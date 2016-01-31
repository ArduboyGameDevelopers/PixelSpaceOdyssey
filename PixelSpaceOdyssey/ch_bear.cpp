#include "ch_bear.h"
#include "enemy_base.h"
#include "game.h"

static const EnemyState EnemyStatePonc = 1;

static inline void ponc(Character *self)
{
    EnemySetState(self, EnemyStatePonc);
}

void EnemyCallbackBear(Character *self, CharacterCallbackType type, int16_t user1, int16_t user2)
{
 
    if (type == CHARACTER_CALLBACK_ANIMATION_FINISHED &&
        EnemyStatePonc == EnemyGetState(self))
    {
        EnemyStat(self);
    }
    else
    {
        EnemyDefaultCallback(self, type, user1, user2);
    }
}

void EnemyBehaviourBear(Character *self, TimeInterval dt)
{
    // TODO: add random ponc
    EnemyDefaultBehaviour(self, dt);
}

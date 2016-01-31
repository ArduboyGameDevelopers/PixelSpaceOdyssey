#include "dog.h"

#include "animation.h"
#include "ch_dog_animations.h"

#include "game.h"



void EnemyInitDog(Character *self)
{
    if (self->initialState == CharacterInitialStatePatrol)
    {
        EnemyPatrol(self);
    }
    else
    {
        EnemyStat(self, false);
    }
}

void EnemyCallbackDog(Character *self, CharacterCallbackType type, int16_t, int16_t)
{
    switch (type)
    {
        case CHARACTER_CALLBACK_ANIMATION_FINISHED:
        {
            EnemyState state = EnemyGetState(self);
            switch (state)
            {
                case EnemyStateAttack:
                    EnemyStat(self);
                    break;
            }
            break;
        }
            
        case CHARACTER_CALLBACK_REACHED_TARGET:
        {
            EnemyState state = EnemyGetState(self);
            switch (state)
            {
                case EnemyStateChase:
                {
                    if (self->canSeePlayer &&
                        EnemyIsCloseToAttack(self) &&
                        EnemyCanAttack(self))
                    {
                        EnemyAttack(self);
                    }
                    else
                    {
                        EnemyStat(self);
                    }
                    break;
                }
                    
                case EnemyStatePatrol:
                {
                    EnemyStat(self);
                    break;
                }
            }
            break;
        }
        
        case CHARACTER_CALLBACK_OBSTACLE:
        {
            EnemyState state = EnemyGetState(self);
            switch (state)
            {
                case EnemyStateChase:
                    EnemyStat(self);
                    break;
            }
            break;
        }
    }
}

void EnemyBehaviourDog(Character *self, TimeInterval dt)
{
    EnemyState state = EnemyGetState(self);
    switch (state)
    {
        case EnemyStateStat:
        case EnemyStatePatrol:
        case EnemyStateChase:
        {
            // update last seen pos
            EnemyUpdatePlayerPos(self);

            // see the player?
            if (self->canSeePlayer)
            {
                int16_t distance = self->lastPlayerX - self->x;  // distance between enemy and last player's seen pos
                self->dir = distance < 0 ? DIR_LEFT : DIR_RIGHT; // face the player

                if (EnemyIsCloseToAttack(self))
                {
                    if (EnemyCanAttack(self))
                    {
                        EnemyAttack(self);
                    }
                    else if (CharacterIsMoving(self))
                    {
                        EnemyStat(self);
                    }
                }
                else if (distance < 0 && EnemyCanMoveLeft(self) || // to far away to attack
                         distance > 0 && EnemyCanMoveRight(self))  // but can move towards the player
                {
                    // set a new target
                    int16_t minDistance = DIV2(self->width + player.width); // how close enemy gets to a player
                    EnemySetTargetPos(self, distance > 0 ? self->lastPlayerX - minDistance : self->lastPlayerX + minDistance);

                    // if not chasing already - start chasing
                    if (state != EnemyStateChase)
                    {
                        EnemyChase(self);
                    }
                }
            }
            else if (state == EnemyStateStat) // stay there and can't see the player - wait until enemy can patrol again
            {
                if (self->stateTime > dt)
                {
                    self->stateTime -= dt;
                }
                else if (self->stateTime > 0)
                {
                    self->stateTime = 0;
                    EnemyPatrol(self);
                }
            }
            break;
        }
    }
}



#include "dispatch.h"

#include "Arduino.h"

static Dispatch *scheduledRoot = NULL;

void DispatchSchedule(DispatchCallback callback, int16_t delay, void *user, int16_t data1, int16_t data2)
{
    assert(callback);
    
    Dispatch *dispatch = DispatchCreate(callback, delay, user, data1, data2);
    dispatch->next = scheduledRoot;
    scheduledRoot = dispatch;
}

void DispatchUpdate(uint16_t dt)
{
    Dispatch *prev = NULL;
    for (Dispatch *current = scheduledRoot; current != NULL;)
    {
        Dispatch *next = current->next;
        current->remains -= dt;
        
        if (current->remains <= 0)
        {
            if (prev) prev->next = next;
            else scheduledRoot = next;
            
            current->callback(current->user, current->data1, current->data2);
            DispatchDestroy(current);
        }
        else
        {
            prev = current;
        }
        current = next;
    }
}

#if TESTING
void DispatchShutdown()
{
    for (Dispatch *dispatch = scheduledRoot; dispatch != NULL;)
    {
        Dispatch *next = dispatch->next;
        DispatchDestroy(dispatch);
        dispatch = next;
    }
    
    scheduledRoot = NULL;
}
#endif // TESTING

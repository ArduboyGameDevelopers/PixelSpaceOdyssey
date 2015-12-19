#ifndef DISPATCH
#define DISPATCH

#include "common.h"

struct _Dispatch;
typedef _Dispatch Dispatch;

typedef void (*DispatchCallback)(void *user, int16_t data1, int16_t data2);

typedef struct _Dispatch {
    Dispatch         *next;
    DispatchCallback callback;
    void             *user;
    int16_t          remains;
    int16_t          data1;
    int16_t          data2;
} Dispatch;

inline Dispatch *DispatchCreate(DispatchCallback callback, int16_t delay, void *user = NULL, int16_t data1 = 0, int16_t data2 = 0)
{
    Dispatch *dispatch  = (Dispatch *) malloc(sizeof(Dispatch));
    dispatch->next      = NULL;
    dispatch->callback  = callback;
    dispatch->user      = user;
    dispatch->remains   = delay;
    dispatch->data1     = data1;
    dispatch->data2     = data2;
    return dispatch;
}

inline void DispatchDestroy(Dispatch *dispatch)
{
    free(dispatch);
}

extern void DispatchSchedule(DispatchCallback callback, int16_t delay, void *user = NULL, int16_t data1 = 0, int16_t data2 = 0);
extern void DispatchUpdate(uint16_t dt);

#if TESTING
extern void DispatchShutdown();
#endif // TESTING

#endif // DISPATCH


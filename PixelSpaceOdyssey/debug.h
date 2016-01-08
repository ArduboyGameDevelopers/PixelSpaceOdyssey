#ifndef DEBUG
#define DEBUG

#define DEBUG_EVENT_PLAYER_DAMAGE "PLAYER_DAMAGE"

#if EMULATOR
    #define DISPATCH_DEBUG_EVENT(NAME) dispatchDebugEvent(NAME)
    void dispatchDebugEvent(const char *name);
#else
    #define DISPATCH_DEBUG_EVENT(NAME)
#endif // EMULATOR

#endif // DEBUG


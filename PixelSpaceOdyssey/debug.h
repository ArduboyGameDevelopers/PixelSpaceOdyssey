#ifndef DEBUG
#define DEBUG

#define DEBUG_EVENT_PLAYER_DAMAGE "PLAYER_DAMAGE"

#if EMULATOR
    #include <qdebug.h>

    #define DISPATCH_DEBUG_EVENT(NAME) dispatchDebugEvent(NAME)
    #define DEBUG_LOG(...) qDebug(__VA_ARGS__)
    void dispatchDebugEvent(const char *name);
#else
    #define DISPATCH_DEBUG_EVENT(NAME)
    #define DEBUG_LOG(...)
#endif // EMULATOR

#endif // DEBUG


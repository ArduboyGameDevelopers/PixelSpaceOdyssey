#ifndef spider_h
#define spider_h

#include "character.h"

#define SPIDER_SLEEP 0
#define SPIDER_AWAKEN 1
#define SPIDER_RISE 2
#define SPIDER_WALK 3

class Spider : public Character
{
private:
    uint8_t _state;
    int8_t  _movement;
    
public:
    Spider() :
        _state(SPIDER_SLEEP),
        _movement(0)
    {
    }
    
public:
    void setState(uint8_t state);
    inline uint8_t state() { return _state; }
    
private:
    void setAnimation(int index);
};

#endif // spider_h

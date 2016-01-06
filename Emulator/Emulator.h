#ifndef EMULATOR_H
#define EMULATOR_H

#include "game.h"

class Emulator
{
public:
    Emulator();

public:
    void start();
    void reset();
    void update(uint32_t dt);
    void stop();

public:
    void buttonPressed(int button);
    void buttonReleased(int button);
    
public:
    inline void step() { _step = true; }
    inline bool editMode() const { return _editMode; }
    inline void setEditMode(bool editMode) { _editMode = editMode; }
    inline bool paused() const { return _paused; }
    inline void setPaused(bool paused) { _paused = paused; }
    
private:
    bool _editMode;
    bool _paused;
    bool _step;
    uint32_t _frameTime;
};

extern Emulator emulator;

#endif // EMULATOR_H

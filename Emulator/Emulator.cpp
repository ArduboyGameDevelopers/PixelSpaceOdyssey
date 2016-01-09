#include "Emulator.h"

#include "platform.h"
#include "bridge.h"

Emulator emulator;

static uint8_t _inputMask = 0;
static unsigned long _gameTime = 0;

static const int kFrameRate = 20;
static const int kFrameTime = 1000 / kFrameRate;

Emulator::Emulator() :
    _editMode(false),
    _paused(false),
    _step(false),
    _frameTime(kFrameTime) // run the first update immediately
{
}

void Emulator::start()
{
    startGame();
}

void Emulator::reset()
{
    _frameTime = kFrameRate;  // run the first update immediately
    _gameTime = 0;
    _inputMask = 0;
}

void Emulator::update(uint32_t dt)
{
    _gameTime += dt;
    _frameTime += dt;

    if (_frameTime >= kFrameTime)
    {
        _frameTime = 0;
        
        if (!_paused || _step)
        {
            updateGame();
            _step = false;
        }
    }
    drawGame();
}

void Emulator::stop()
{
}

void Emulator::buttonPressed(int button)
{
    _inputMask |= button;
}

void Emulator::buttonReleased(int button)
{
    _inputMask &= ~button;
}

unsigned long platformMillis(void)
{
    return _gameTime;
}

void platformDelay(unsigned long)
{
}

uint8_t platformInput()
{
    return _inputMask;
}

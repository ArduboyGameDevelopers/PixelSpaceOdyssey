#include "Emulator.h"

#include "platform.h"
#include "bridge.h"

Emulator emulator;

static uint8_t _inputMask = 0;
static unsigned long _gameTime = 0;

Emulator::Emulator() :
    _editMode(false),
    _paused(false),
    _step(false)
{
}

void Emulator::start()
{
    startGame();
}

void Emulator::update(uint32_t dt)
{
    _gameTime += dt;

    if (!_paused || _step)
    {
        updateGame();
        _step = false;
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

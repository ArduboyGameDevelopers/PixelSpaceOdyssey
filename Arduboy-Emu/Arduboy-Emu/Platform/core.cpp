#include "core.h"

ArduboyCore::ArduboyCore() {}

// meant to be overridden by subclasses
void ArduboyCore::setup()
{
  boot();
}

void ArduboyCore::boot()
{
}

void ArduboyCore::bootPins()
{
}

void ArduboyCore::bootLCD()
{
}

void ArduboyCore::LCDDataMode()
{
}

void ArduboyCore::LCDCommandMode()
{
}

void ArduboyCore::safeMode()
{
}

/* Power Management */

void ArduboyCore::idle()
{
}

void ArduboyCore::saveMuchPower()
{
}

uint8_t ArduboyCore::width() { return WIDTH; }

uint8_t ArduboyCore::height() { return HEIGHT; }


/* Drawing */

void ArduboyCore::paint8Pixels(uint8_t pixels)
{
}

void ArduboyCore::paintScreen(const unsigned char *image)
{
}

// paint from a memory buffer, this should be FAST as it's likely what
// will be used by any buffer based subclass
void ArduboyCore::paintScreen(unsigned char image[])
{
}

void ArduboyCore::blank()
{
}

/* Buttons */

uint8_t ArduboyCore::getInput()
{
  return 0;
}

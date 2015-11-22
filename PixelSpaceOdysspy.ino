#include <SPI.h>
#include <EEPROM.h>
#include "Arduboy.h"
#include "animation.h"
#include "images.h"

Arduboy display;

extern void DrawFrame(FrameData data, uint8_t x, uint8_t y);

int frame = 0;

void setup() {
  SPI.begin();
  display.setFrameRate(10);
  display.start();
}

void loop() {
  if (!display.nextFrame()) return;

  frame = (frame + 1) % animation_walk.frameCount;
  
  display.clearDisplay();
  DrawFrame(animation_walk.frames[frame], 0, 0);
}

void DrawFrame(FrameData data, uint8_t x, uint8_t y)
{
  uint8_t offsetX = pgm_read_byte(data);
  uint8_t offsetY = pgm_read_byte(data + 1);
  uint8_t width   = pgm_read_byte(data + 2);
  uint8_t height  = pgm_read_byte(data + 3);

  FrameData ptr = data + 4;
  display.drawBitmap(x + offsetX, y + offsetY, ptr, width, height, WHITE);
  display.display();
}

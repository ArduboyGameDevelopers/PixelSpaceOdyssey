#include <SPI.h>

#include "game.h"

void setup() {
  SPI.begin();
  startGame();
}

void loop() {
  loopGame();
}

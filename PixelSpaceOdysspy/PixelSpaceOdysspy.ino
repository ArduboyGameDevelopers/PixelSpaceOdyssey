#include <SPI.h>

#include "bridge.h"

void setup() {
  SPI.begin();
  startGame();
}

void loop() {
  loopGame();
}

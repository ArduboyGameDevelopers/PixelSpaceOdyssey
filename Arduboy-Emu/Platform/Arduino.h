#ifndef ARDUINO_H__
#define ARDUINO_H__

#include <types.h>

#define _BV(bit) (1 << (bit))
#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))

unsigned long millis(void);

#endif // ARDUINO_H__
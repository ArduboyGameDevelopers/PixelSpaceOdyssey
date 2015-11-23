#ifndef ARDUINO_H__
#define ARDUINO_H__

#define _BV(bit) (1 << (bit))
#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))

typedef unsigned char  uint8_t;
typedef unsigned short uint16_t;
typedef unsigned char  byte;
typedef bool boolean;

unsigned long millis(void);

#endif // ARDUINO_H__
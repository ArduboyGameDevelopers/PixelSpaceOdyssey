//
//  pgmspace.h
//  Arduboy-Emu
//
//  Created by Alex Lementuev on 11/22/15.
//  Copyright © 2015 Space Madness. All rights reserved.
//

#ifndef pgmspace_h
#define pgmspace_h

#include <assert.h>

#define PROGMEM

inline unsigned char pgm_read_byte(const unsigned char *ptr)
{
    assert(ptr);
    return *ptr;
}

#endif /* pgmspace_h */

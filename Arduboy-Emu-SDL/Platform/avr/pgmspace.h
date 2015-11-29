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

#define pgm_read_byte(address_short) (*(address_short))
#define pgm_read_ptr(address_short)  (address_short)

#endif /* pgmspace_h */

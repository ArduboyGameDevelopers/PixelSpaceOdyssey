//
//  Arduino.cpp
//  Arduboy-Emu
//
//  Created by Alex Lementuev on 11/22/15.
//  Copyright © 2015 Space Madness. All rights reserved.
//

#include <time.h>

#include "Arduino.h"

unsigned long millis(void)
{
    time_t time = clock();
    return time * CLOCKS_PER_SEC / 1000;
}
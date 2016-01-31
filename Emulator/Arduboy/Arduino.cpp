//
//  Arduino.cpp
//  Arduboy-Emu
//
//  Created by Alex Lementuev on 11/22/15.
//  Copyright © 2015 Space Madness. All rights reserved.
//

#include <stdlib.h>

#include "platform.h"
#include "Arduino.h"

unsigned long millis(void)
{
    return platformMillis();
}

void delay(unsigned long millis)
{
    platformDelay(millis);
}

long random(long howbig)
{
    if ( howbig == 0 )
    {
        return 0 ;
    }
    
    return rand() % howbig;
}

long random(long howsmall, long howbig)
{
    if (howsmall >= howbig)
    {
        return howsmall;
    }
    
    long diff = howbig - howsmall;
    
    return random(diff) + howsmall;
}
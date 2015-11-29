//
//  Arduino.cpp
//  Arduboy-Emu
//
//  Created by Alex Lementuev on 11/22/15.
//  Copyright © 2015 Space Madness. All rights reserved.
//

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
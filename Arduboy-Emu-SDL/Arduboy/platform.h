//
//  platform.h
//  Arduboy-Emu
//
//  Created by Alex Lementuev on 11/22/15.
//  Copyright © 2015 Space Madness. All rights reserved.
//

#ifndef platform_h__
#define platform_h__

#include <types.h>

#ifdef __cplusplus
extern "C" {
#endif

extern void platformRenderScreen(unsigned const char* screenBuffer, int width, int height);
extern unsigned long platformMillis(void);
extern void platformDelay(unsigned long millis);
extern uint8_t platformInput();
    
#ifdef __cplusplus
}
#endif

#endif /* platform_h__ */

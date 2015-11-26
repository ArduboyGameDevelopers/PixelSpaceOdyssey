//
//  drawing.h
//  PixelSpaceOdessey
//
//  Created by Alex Lementuev on 11/26/15.
//  Copyright © 2015 Space Madness. All rights reserved.
//

#ifndef drawing_h
#define drawing_h

#include "common.h"

void drawInit(uint8_t *buffer);
void drawImage(PgmPtr image, int16_t x, int16_t y, uint8_t w, uint8_t h, DrawMode mode = DM_UNLIT);

#endif /* drawing_h */

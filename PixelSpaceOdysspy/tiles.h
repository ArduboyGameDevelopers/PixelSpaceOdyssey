//
//  tilemap.h
//  PixelSpaceOdessey
//
//  Created by Alex Lementuev on 11/24/15.
//  Copyright © 2015 Space Madness. All rights reserved.
//

#ifndef tilemap_h
#define tilemap_h

typedef const uint8_t* tile_t;

#define TILES_COUNT 19
extern tile_t const tiles[];

#define MAP_WIDTH 19
#define MAP_HEIGHT 13
#define INDEX_COUNT 247
extern uint8_t const indices[];
    
#endif /* tilemap_h */
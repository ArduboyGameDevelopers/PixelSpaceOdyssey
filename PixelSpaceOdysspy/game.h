//
//  game.h
//  PixelSpaceOdessey
//
//  Created by Alex Lementuev on 11/24/15.
//  Copyright © 2015 Space Madness. All rights reserved.
//

#ifndef game_h
#define game_h

#define RUN_BUTTON  A_BUTTON
#define JUMP_BUTTON B_BUTTON

#define WORLD_TO_SCREEN(X) ((X) >> 4)
#define SCREEN_TO_WORLD(X) ((X) << 4)

#define GRAVITY 50
#define FLOOR   416

#define JUMP_SPEED -200
#define WALK_SPEED 32

#define MIN_X 0
#define MAX_X 1824

extern short x, y;
extern bool jumping;
extern bool crouching;
extern short camX, camY;

#endif /* game_h */

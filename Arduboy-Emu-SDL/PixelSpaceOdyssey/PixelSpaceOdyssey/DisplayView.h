//
//  DisplayView.h
//  PixelSpaceOdyssey
//
//  Created by Alex Lementuev on 11/29/15.
//  Copyright © 2015 Space Madness. All rights reserved.
//

#ifndef DisplayView_h
#define DisplayView_h

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

class DisplayView
{
private:
    SDL_Renderer * _renderer;
    SDL_Rect     * _pixelRects;
    
public:
    DisplayView(SDL_Renderer* renderer);
    ~DisplayView();
    
public:
    void renderScreenBuffer(unsigned const char* screenBuffer, int bufferWidth, int bufferHeight);
};

#endif /* DisplayView_h */

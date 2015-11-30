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
    SDL_Rect     * _pixelRects;
    int            _pixelRectsCount;
    int            _width;
    int            _height;
    
public:
    DisplayView(int width, int height);
    ~DisplayView();
    
public:
    void copyScreenBuffer(unsigned const char* screenBuffer, int bufferWidth, int bufferHeight);
    void render(SDL_Renderer* renderer);
};

#endif /* DisplayView_h */

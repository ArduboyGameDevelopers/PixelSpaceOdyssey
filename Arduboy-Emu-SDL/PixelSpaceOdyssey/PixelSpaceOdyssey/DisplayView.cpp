//
//  DisplayView.c
//  PixelSpaceOdyssey
//
//  Created by Alex Lementuev on 11/29/15.
//  Copyright © 2015 Space Madness. All rights reserved.
//

#include "DisplayView.h"

DisplayView::DisplayView(int width, int height) :
    _pixelRects(NULL),
    _pixelRectsCount(0),
    _width(width),
    _height(height)
{
}

DisplayView::~DisplayView()
{
    if (_pixelRects)
    {
        free(_pixelRects);
    }
}

void DisplayView::copyScreenBuffer(unsigned const char* screenBuffer, int bufferWidth, int bufferHeight)
{
    if (_pixelRects == NULL)
    {
        _pixelRects = (SDL_Rect *) malloc(bufferWidth * bufferHeight * sizeof(SDL_Rect));
    }
    
    int pixelWidth = _width / bufferWidth;
    int pixelHeight = _height / bufferHeight;
    
    _pixelRectsCount = 0;
    for (int y = 0; y < bufferHeight; y++)
    {
        for (int x = 0; x < bufferWidth; x++)
        {
            int row = y / 8;
            int index = row * bufferWidth + x;
            int bit = screenBuffer[index] & (1 << (y % 8));
            if (!bit)
            {
                SDL_Rect pixelRect;
                pixelRect.x = x * pixelWidth;
                pixelRect.y = y * pixelHeight;
                pixelRect.w = pixelWidth;
                pixelRect.h = pixelHeight;
                _pixelRects[_pixelRectsCount++] = pixelRect;
            }
        }
    }
}

void DisplayView::render(SDL_Renderer* renderer)
{
    if (_pixelRectsCount > 0)
    {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderFillRects(renderer, _pixelRects, _pixelRectsCount);
    }
}
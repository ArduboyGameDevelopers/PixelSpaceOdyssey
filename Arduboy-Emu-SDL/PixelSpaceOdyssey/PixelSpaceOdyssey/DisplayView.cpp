//
//  DisplayView.c
//  PixelSpaceOdyssey
//
//  Created by Alex Lementuev on 11/29/15.
//  Copyright © 2015 Space Madness. All rights reserved.
//

#include "DisplayView.h"

DisplayView::DisplayView(SDL_Renderer *renderer) :
    _renderer(renderer),
    _pixelRects(NULL)
{
}

DisplayView::~DisplayView()
{
    if (_pixelRects)
    {
        free(_pixelRects);
    }
}

void DisplayView::renderScreenBuffer(unsigned const char* screenBuffer, int bufferWidth, int bufferHeight)
{
    if (_pixelRects == NULL)
    {
        _pixelRects = (SDL_Rect *) malloc(bufferWidth * bufferHeight * sizeof(SDL_Rect));
    }
    
    int width, height;
    SDL_GetRendererOutputSize(_renderer, &width, &height);
    
    int pixelWidth = width / bufferWidth;
    int pixelHeight = height / bufferHeight;
    
    int pixelRectsCount = 0;
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
                _pixelRects[pixelRectsCount++] = pixelRect;
            }
        }
    }
    
    SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
    SDL_RenderFillRects(_renderer, _pixelRects, pixelRectsCount);
}
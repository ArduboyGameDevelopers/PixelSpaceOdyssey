//
//  DisplayView.c
//  PixelSpaceOdyssey
//
//  Created by Alex Lementuev on 11/29/15.
//  Copyright © 2015 Space Madness. All rights reserved.
//

#include "DisplayView.h"

#include "drawing.h"
#include "game.h"

struct RectList {
    SDL_Rect* rects;
    int count;
};

static const int GRID_CELL_SIZE = 8;

DisplayView::DisplayView(int width, int height) :
    View(width, height),
    _pixelRects(NULL),
    _gridRects(NULL),
    _gridVisible(false)
{
}

DisplayView::~DisplayView()
{
    RectListDestroy(_pixelRects);
    RectListDestroy(_gridRects);
}

void DisplayView::copyScreenBuffer(unsigned const char* screenBuffer, int bufferWidth, int bufferHeight)
{
    if (_pixelRects == NULL)
    {
        _pixelRects = RectListCreate(bufferWidth * bufferHeight);
    }
    
    int pixelWidth = width() / bufferWidth;
    int pixelHeight = height() / bufferHeight;
    
    _pixelRects->count = 0;
    for (int y = 0; y < bufferHeight; y++)
    {
        for (int x = 0; x < bufferWidth; x++)
        {
            int row = y / 8;
            int index = row * bufferWidth + x;
            int bit = screenBuffer[index] & (1 << (y % 8));
            if (!bit)
            {
                SDL_Rect& rect = _pixelRects->rects[_pixelRects->count++];
                rect.x = x * pixelWidth;
                rect.y = y * pixelHeight;
                rect.w = pixelWidth;
                rect.h = pixelHeight;
            }
        }
    }
    
    if (_gridVisible)
    {
        int gridRows = tileMapHeight / GRID_CELL_SIZE;
        int gridCols = tileMapWidth / GRID_CELL_SIZE;
        if (_gridRects == NULL)
        {
            _gridRects = RectListCreate((gridRows + 1) * (gridCols + 1));
        }
        
        bool cellVisible = false;
        int cellWidth = GRID_CELL_SIZE * pixelWidth;
        int cellHeight = GRID_CELL_SIZE * pixelHeight;
        int offsetX = drawTransX * pixelWidth;
        int offsetY = drawTransY * pixelHeight;
        
        _gridRects->count = 0;
        for (int i = 0; i <= gridRows; ++i)
        {
            for (int j = 0; j <= gridCols; ++j)
            {
                if (cellVisible)
                {
                    SDL_Rect& rect = _gridRects->rects[_gridRects->count++];
                    rect.x = offsetX + j * cellWidth;
                    rect.y = offsetY + i * cellHeight;
                    rect.w = cellWidth;
                    rect.h = cellHeight;
                }
                cellVisible = !cellVisible;
            }
        }
    }
}

void DisplayView::render(SDL_Renderer* renderer)
{
    // render grid
    if (_gridVisible)
    {
        RectListRender(renderer, _gridRects, 241, 245, 248, 255);
    }

    // render pixels
    RectListRender(renderer, _pixelRects, 0, 0, 0, 255);
}

RectList* RectListCreate(int capacity)
{
    RectList *list = (RectList *)malloc(sizeof(RectList));
    list->count = 0;
    list->rects = (SDL_Rect *)malloc(capacity * sizeof(SDL_Rect));
    return list;
}

void RectListRender(SDL_Renderer* renderer, const RectList* list, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    if (list && list->count > 0)
    {
        SDL_SetRenderDrawColor(renderer, r, g, b, a);
        SDL_RenderFillRects(renderer, list->rects, list->count);
    }
}

void RectListDestroy(RectList* list)
{
    if (list)
    {
        free(list->rects);
        free(list);
    }
}
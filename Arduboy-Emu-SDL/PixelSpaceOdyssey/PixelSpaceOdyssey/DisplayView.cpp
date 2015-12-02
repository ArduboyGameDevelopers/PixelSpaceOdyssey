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

#include "EditorState.h"
#include "tiles_lair_01.h"

struct RectList {
    SDL_Rect* rects;
    int count;
};

static const int GRID_CELL_SIZE = 8;
static const int PIXEL_WIDTH    = 4;
static const int PIXEL_HEIGHT   = 4;

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
                rect.x = left() + x * PIXEL_WIDTH;
                rect.y = top() + y * PIXEL_HEIGHT;
                rect.w = PIXEL_WIDTH;
                rect.h = PIXEL_HEIGHT;
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
        int cellWidth = GRID_CELL_SIZE * PIXEL_WIDTH;
        int cellHeight = GRID_CELL_SIZE * PIXEL_HEIGHT;
        int offsetX = left() + drawTransX * PIXEL_WIDTH;
        int offsetY = top() + drawTransY * PIXEL_HEIGHT;
        
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

void DisplayView::render(SDL_Renderer* render) const
{
    // render grid
    if (_gridVisible)
    {
        RectListRender(render, _gridRects, 241, 245, 248, 255);
    }

    // render pixels
    RectListRender(render, _pixelRects, 0, 0, 0, 255);
}

void DisplayView::mouseDown(int x, int y)
{
    if (editorState.tileIndex != -1)
    {
        int gridIndex = gridIndexFromCords(x, y);
        uint8_t* indicesPtr = (uint8_t*)(void*)INDICES_LAIR_01; // evil C
        indicesPtr[gridIndex] = editorState.tileIndex;
    }
}

void DisplayView::mouseMove(int x, int y)
{
    
}

void DisplayView::mouseUp(int x, int y)
{
    
}

int DisplayView::gridIndexFromCords(int x, int y)
{
    int col = (x / PIXEL_WIDTH - drawTransX) / GRID_CELL_SIZE;
    int row = (y / PIXEL_HEIGHT - drawTransY) / GRID_CELL_SIZE;
    int width = W2S(tileMapWidth);
    
    int colCount = width / GRID_CELL_SIZE;
    return colCount * row + col;
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
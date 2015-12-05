//
//  TileView.c
//  PixelSpaceOdyssey
//
//  Created by Alex Lementuev on 11/29/15.
//  Copyright © 2015 Space Madness. All rights reserved.
//

#include "TileView.h"
#include "tiles_lair_01.h"

#include "EditorState.h"

static const int ROW_SIZES[] = { 13, 2, 4, 2, 2 };

TileView::TileView(TileSet* tileSet, int width, int height) :
    View(width, height),
    _tileSet(tileSet->retain()),
    _selectedIndex(-1)
{
}

void TileView::render(SDL_Renderer* render) const
{
    SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
    SDL_RenderFillRect(render, &_rect);
    
    int tileWidth = _tileSet->tileWidth();
    int tileHeight = _tileSet->tileHeight();
    for (int tileIndex = 0, k = 0, row = 0, col = 0; tileIndex < _tileSet->tileCount(); ++tileIndex, ++col, ++k)
    {
        if (ROW_SIZES[row] <= k)
        {
            k = 0;
            col = 0;
            ++row;
        }
        
        int x = kBorder + left() + col * (tileWidth + kBorder);
        int y = top() + kBorder + row * (tileHeight + kBorder);
        
        if (tileIndex == _selectedIndex)
        {
            SDL_Rect rect = { x - kBorder, y - kBorder, tileWidth + 2 * kBorder, tileHeight + 2 * kBorder };
            SDL_SetRenderDrawColor(render, 255, 0, 255, 255);
            SDL_RenderFillRect(render, &rect);
        }
        
        _tileSet->drawTile(render, tileIndex, x, y);
    }
}

void TileView::mouseDown(int x, int y)
{
    _selectedIndex = tileIndexFromCords(x, y);
    editorState.tileIndex = _selectedIndex + 1;
}

int TileView::tileIndexFromCords(int x, int y) const
{
    int col = (x - kBorder) / (_tileSet->tileWidth() + kBorder);
    int row = (y - kBorder) / (_tileSet->tileHeight() + kBorder);
    
    int indicesLength = sizeof(ROW_SIZES) / sizeof(ROW_SIZES[0]);
    if (row >= 0 && row < indicesLength)
    {
        int size = ROW_SIZES[row];
        if (col >= 0 && col < size)
        {
            int index = col;
            for (int i = 0; i < row; ++i)
            {
                index += ROW_SIZES[i];
            }
            
            return index;
        }
    }
    
    return -1;
}
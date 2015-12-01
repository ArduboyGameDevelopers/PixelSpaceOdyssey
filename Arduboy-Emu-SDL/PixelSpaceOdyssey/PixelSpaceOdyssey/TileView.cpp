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

TileView::TileView(SDL_Texture* tilesTexture, int width, int height) :
    View(width, height),
    _tilesTexture(tilesTexture),
    _selectedIndex(0)
{
    int tw, th;
    SDL_QueryTexture(tilesTexture, NULL, NULL, &tw, &th);
    
    assert(tw % th == 0);
    _tileCount = tw / th;
    _tileSize = th;
}

void TileView::render(SDL_Renderer* render) const
{
    SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
    SDL_RenderFillRect(render, &_rect);
    
    SDL_Rect src = { 0, 0, _tileSize, _tileSize };
    SDL_Rect dst = { 0, 0, _tileSize, _tileSize };
    
    for (int tileIndex = 0, k = 0, row = 0, col = 0; tileIndex < _tileCount; ++tileIndex, ++col, ++k)
    {
        if (ROW_SIZES[row] <= k)
        {
            k = 0;
            col = 0;
            ++row;
        }
        
        dst.x = kBorder + left() + col * (_tileSize + kBorder);
        dst.y = top() + kBorder + row * (_tileSize + kBorder);
        
        if (tileIndex == _selectedIndex)
        {
            SDL_Rect rect = { dst.x - kBorder, dst.y - kBorder, _tileSize + 2 * kBorder, _tileSize + 2 * kBorder };
            SDL_SetRenderDrawColor(render, 255, 0, 255, 255);
            SDL_RenderFillRect(render, &rect);
        }
        
        src.x = tileIndex * _tileSize;
        SDL_RenderCopy(render, _tilesTexture, &src, &dst);
    }
}

void TileView::mouseDown(int x, int y)
{
    _selectedIndex = tileIndexFromCords(x, y);
    editorState.tileIndex = _selectedIndex + 1;
}

int TileView::tileIndexFromCords(int x, int y) const
{
    int col = (x - kBorder) / (_tileSize + kBorder);
    int row = (y - kBorder) / (_tileSize + kBorder);
    
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
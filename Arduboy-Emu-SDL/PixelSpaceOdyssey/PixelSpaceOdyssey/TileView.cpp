//
//  TileView.c
//  PixelSpaceOdyssey
//
//  Created by Alex Lementuev on 11/29/15.
//  Copyright © 2015 Space Madness. All rights reserved.
//

#include "TileView.h"

static const int INDICES[] = { 13, 2, 4, 2, 2 };

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
    SDL_RenderFillRect(render, &_bounds);
    
    SDL_Rect src = { 0, 0, _tileSize, _tileSize };
    SDL_Rect dst = { 0, 0, _tileSize, _tileSize };
    
    for (int i = 0, k = 0, row = 0, col = 0; i < _tileCount; ++i, ++col, ++k)
    {
        if (INDICES[row] <= k)
        {
            k = 0;
            col = 0;
            ++row;
        }
        
        dst.x = kBorder + left() + col * (_tileSize + kBorder);
        dst.y = top() + kBorder + row * (_tileSize + kBorder);
        
        src.x = i * _tileSize;
        SDL_RenderCopy(render, _tilesTexture, &src, &dst);
    }
}
//
//  TileSet.c
//  PixelSpaceOdyssey
//
//  Created by Alex Lementuev on 11/30/15.
//  Copyright © 2015 Space Madness. All rights reserved.
//

#include <assert.h>

#include "TileSet.h"

TileSet::TileSet(SDL_Texture* tileTexture) :
    _tileTexture(tileTexture)
{
    int w, h;
    SDL_QueryTexture(tileTexture, NULL, NULL, &w, &h);
    
    assert(w % h == 0);
    
    _tileHeight = h;
    _tileWidth = h;
    _tileCount = w / h;
}

void TileSet::drawTile(SDL_Renderer* renderer, int tileIndex, int x, int y)
{
    assert(tileIndex >= 0 && tileIndex < _tileCount);

    SDL_Rect src = { tileIndex * _tileWidth, 0, _tileWidth, _tileHeight };
    SDL_Rect dst = { x, y, _tileWidth, _tileHeight };
    SDL_RenderCopy(renderer, _tileTexture, &src, &dst);
}
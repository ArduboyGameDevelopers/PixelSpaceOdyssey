//
//  TileSet.c
//  PixelSpaceOdyssey
//
//  Created by Alex Lementuev on 11/30/15.
//  Copyright © 2015 Space Madness. All rights reserved.
//

#include <assert.h>

#include "TileSet.h"

TileSet::TileSet(Texture* tileTexture) :
    _tileTexture(tileTexture->retain())
{
    _tileHeight = tileTexture->height();
    _tileWidth = tileTexture->height();
    _tileCount = tileTexture->width() / tileTexture->height();
}

TileSet::~TileSet()
{
    _tileTexture->release();
}

void TileSet::drawTile(SDL_Renderer* renderer, int tileIndex, int x, int y)
{
    assert(tileIndex >= 0 && tileIndex < _tileCount);

    SDL_Rect src = { tileIndex * _tileWidth, 0, _tileWidth, _tileHeight };
    SDL_Rect dst = { x, y, _tileWidth, _tileHeight };
    SDL_RenderCopy(renderer, _tileTexture->texture(), &src, &dst);
}
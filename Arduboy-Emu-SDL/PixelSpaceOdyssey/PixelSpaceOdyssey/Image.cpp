//
//  Image.c
//  PixelSpaceOdyssey
//
//  Created by Alex Lementuev on 12/1/15.
//  Copyright © 2015 Space Madness. All rights reserved.
//

#include <assert.h>

#include "Image.h"

Image::Image(Texture* texture) :
    _texture(texture->retain())
{
    _sourceRect.x = 0;
    _sourceRect.y = 0;
    _sourceRect.w = texture->width();
    _sourceRect.h = texture->height();
}

Image::Image(Texture* texture, int srcX, int srcY, int srcW, int srcH) :
    _texture(texture->retain())
{
    int w = texture->width();
    int h = texture->height();
    assert(srcX >= 0 && srcX + srcW <= w && srcY >= 0 && srcY + srcH <= h);
    
    _sourceRect.x = srcX;
    _sourceRect.y = srcY;
    _sourceRect.w = srcW;
    _sourceRect.h = srcH;
}

Image::~Image()
{
    _texture->release();
}

void Image::draw(SDL_Renderer* renderer, int x, int y)
{
    SDL_Rect dstRect = { x, y, width(), height() };
    SDL_RenderCopy(renderer, _texture->texture(), &_sourceRect, &dstRect);
}
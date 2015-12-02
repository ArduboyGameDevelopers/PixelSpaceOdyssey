//
//  Texture.c
//  PixelSpaceOdyssey
//
//  Created by Alex Lementuev on 12/1/15.
//  Copyright © 2015 Space Madness. All rights reserved.
//

#include <assert.h>

#include "Texture.h"

Texture::Texture(SDL_Texture* texture) :
    _texture(texture)
{
    int w, h;
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    _width = w;
    _height = h;
}

Texture::~Texture()
{
    SDL_DestroyTexture(_texture);
}

Texture* Texture::load(SDL_Renderer *renderer, const char *filename)
{
    SDL_Surface *surface = SDL_LoadBMP(filename);
    assert(surface);
    
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    
    return new Texture(texture);
}
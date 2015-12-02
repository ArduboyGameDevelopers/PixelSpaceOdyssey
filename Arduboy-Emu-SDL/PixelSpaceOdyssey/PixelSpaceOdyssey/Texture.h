//
//  Texture.h
//  PixelSpaceOdyssey
//
//  Created by Alex Lementuev on 12/1/15.
//  Copyright © 2015 Space Madness. All rights reserved.
//

#ifndef Texture_h
#define Texture_h

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include "Object.h"

class Texture : public Object
{
OBJECT_DEF(Texture)
    
private:
    SDL_Texture* _texture;
    int _width;
    int _height;
    
public:
    Texture(SDL_Texture* texture);
    virtual ~Texture();
    
public:
    static Texture* load(SDL_Renderer* renderer, const char* filename);
    
public:
    inline SDL_Texture* texture() const { return _texture; }
    inline int width() const { return _width; }
    inline int height() const { return _height; }
};

#endif /* Texture_h */

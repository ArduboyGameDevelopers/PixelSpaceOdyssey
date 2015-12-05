//
//  TileSet.h
//  PixelSpaceOdyssey
//
//  Created by Alex Lementuev on 11/30/15.
//  Copyright © 2015 Space Madness. All rights reserved.
//

#ifndef TileSet_h
#define TileSet_h

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include "Object.h"
#include "Texture.h"

class TileSet : public Object
{
OBJECT_DEF(TileSet);
    
private:
    Texture* _tileTexture;
    int _tileWidth;
    int _tileHeight;
    int _tileCount;
    
public:
    TileSet(Texture* tileTexture);
    virtual ~TileSet();
    
public:
    void drawTile(SDL_Renderer* renderer, int tileIndex, int x, int y);
    
public:
    inline int tileCount() const { return _tileCount; }
    inline int tileWidth() const { return _tileWidth; }
    inline int tileHeight() const { return _tileHeight; }
};

#endif /* TileSet_h */

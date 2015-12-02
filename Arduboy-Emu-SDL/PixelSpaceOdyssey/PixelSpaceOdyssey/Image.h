//
//  Image.h
//  PixelSpaceOdyssey
//
//  Created by Alex Lementuev on 12/1/15.
//  Copyright © 2015 Space Madness. All rights reserved.
//

#ifndef Image_h
#define Image_h

#include "Object.h"
#include "Texture.h"

class Image : public Object
{
OBJECT_DEF(Image)
    
private:
    Texture* _texture;
    SDL_Rect _sourceRect;
    
public:
    Image(Texture* texture);
    Image(Texture* texture, int srcX, int srcY, int srcW, int srcH);
    virtual ~Image();
    
public:
    void draw(SDL_Renderer* renderer, int x, int y);
    
public:
    inline int width() const { return _sourceRect.w; }
    inline int height() const { return _sourceRect.h; }
};


#endif /* Image_h */

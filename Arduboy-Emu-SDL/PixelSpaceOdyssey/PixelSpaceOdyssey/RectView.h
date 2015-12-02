//
//  RectView.h
//  PixelSpaceOdyssey
//
//  Created by Alex Lementuev on 12/1/15.
//  Copyright © 2015 Space Madness. All rights reserved.
//

#ifndef RectView_h
#define RectView_h

#include "View.h"

class RectView : public View
{
OBJECT_DEF(RectView)
    
private:
    Uint8 _r;
    Uint8 _g;
    Uint8 _b;
    
public:
    RectView(int width, int height, Uint8 r, Uint8 g, Uint8 b);
    
public:
    virtual void render(SDL_Renderer* render) const;
};

#endif /* RectView_h */

//
//  RectView.c
//  PixelSpaceOdyssey
//
//  Created by Alex Lementuev on 12/1/15.
//  Copyright © 2015 Space Madness. All rights reserved.
//

#include "RectView.h"

RectView::RectView(int width, int height, Uint8 r, Uint8 g, Uint8 b) :
    View(width, height),
    _r(r),
    _g(g),
    _b(b)
{
}

void RectView::render(SDL_Renderer* render) const
{
    SDL_SetRenderDrawColor(render, _r, _g, _b, 255);
    SDL_RenderFillRect(render, &_rect);
}
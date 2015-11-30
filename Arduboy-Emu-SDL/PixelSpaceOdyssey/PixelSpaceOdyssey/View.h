//
//  View.h
//  PixelSpaceOdyssey
//
//  Created by Alex Lementuev on 11/29/15.
//  Copyright © 2015 Space Madness. All rights reserved.
//

#ifndef View_h
#define View_h

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include "List.h"

class View
{
protected:
    SDL_Rect _bounds;
    
public:
    View()
    {
        _bounds = { 0, 0, 0, 0 };
    }
    
    View(int width, int height)
    {
        _bounds = { 0, 0, width, height };
    }
    
public:
    virtual void render(SDL_Renderer* render) const {}
    
    virtual void mouseDown(int x, int y) {}
    virtual void mouseMove(int x, int y) {}
    virtual void mouseUp(int x, int y) {}
    
    virtual void keyDown() {}
    virtual void keyUp() {}
    
public:
    inline int top()    const { return _bounds.y; }
    inline int left()   const { return _bounds.x; }
    inline int right()  const { return _bounds.x + _bounds.w; }
    inline int bottom() const { return _bounds.y + _bounds.h; }
    inline int width()  const { return _bounds.w; }
    inline int height() const { return _bounds.h; }
    
    inline void setPos(int x, int y)  { _bounds.x = x; _bounds.y = y; }
    inline void setSize(int w, int h) { _bounds.w = w; _bounds.h = h; }
    
    inline bool containsPoint(int x, int y) const { return x >= left() && x < right() && y >= top() && y < bottom(); }
};

#endif /* View_h */

//
//  RootView.c
//  PixelSpaceOdyssey
//
//  Created by Alex Lementuev on 11/30/15.
//  Copyright © 2015 Space Madness. All rights reserved.
//

#include "RootView.h"

RootView::RootView(int width, int height) :
    View(width, height),
    _mouseOverView(NULL)
{
    _children = new List<View *>(32);
}

RootView::~RootView()
{
    for (int i = 0; i < _children->size(); ++i)
    {
        _children->get(i)->release();
    }
    delete _children;
}

void RootView::addView(View* view)
{
    _children->add(view->retain());
}

void RootView::update(Uint32 ticks)
{
    for (int i = 0; i < _children->size(); ++i)
    {
        _children->get(i)->update(ticks);
    }
}

void RootView::render(SDL_Renderer* render) const
{
    for (int i = 0; i < _children->size(); ++i)
    {
        View* view = _children->get(i);
        SDL_Rect viewRect = view->rect();
        SDL_RenderSetClipRect(render, &viewRect);
        view->render(render);
    }
    SDL_RenderSetClipRect(render, &_rect);
}

void RootView::mouseDown(int x, int y)
{
    View *view = findMouseView(x, y);
    if (view)
    {
        int localX = x - view->left();
        int localY = y - view->top();
        view->mouseDown(localX, localY);
    }
}

void RootView::mouseMove(int x, int y)
{
    View *view = findMouseView(x, y);
    if (_mouseOverView != view)
    {
        if (_mouseOverView)
        {
            int localX = x - _mouseOverView->left();
            int localY = y - _mouseOverView->top();
            _mouseOverView->setHasMouse(false);
            _mouseOverView->mouseExit(localY, localX);
        }
        
        if (view)
        {
            int localX = x - view->left();
            int localY = y - view->top();
            view->setHasMouse(true);
            view->mouseEnter(localX, localY);
        }
        
        _mouseOverView = view;
    }
    
    if (view)
    {
        int localX = x - view->left();
        int localY = y - view->top();
        view->mouseMove(localX, localY);
    }
}

void RootView::mouseUp(int x, int y)
{
    View *view = findMouseView(x, y);
    if (view)
    {
        int localX = x - view->left();
        int localY = y - view->top();
        view->mouseUp(localX, localY);
    }
}

View* RootView::findMouseView(int x, int y)
{
    for (int i = _children->size() - 1; i >= 0; --i)
    {
        View* view = _children->get(i);
        if (view->containsPoint(x, y))
        {
            return view;
        }
    }
    
    return NULL;
}
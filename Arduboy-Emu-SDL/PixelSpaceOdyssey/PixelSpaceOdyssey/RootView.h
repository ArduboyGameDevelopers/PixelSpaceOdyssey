//
//  RootView.h
//  PixelSpaceOdyssey
//
//  Created by Alex Lementuev on 11/30/15.
//  Copyright © 2015 Space Madness. All rights reserved.
//

#ifndef RootView_h
#define RootView_h

#include "View.h"
#include "List.h"

class RootView : public View
{
private:
    List<View *>* _children;
    
public:
    RootView(int width, int height);
    virtual ~RootView();

public:
    void addView(View* view);
    
public:
    virtual void render(SDL_Renderer* render) const;
    
    virtual void mouseDown(int x, int y);
    virtual void mouseMove(int x, int y);
    virtual void mouseUp(int x, int y);
    
private:
    View* findMouseView(int x, int y);
};

#endif /* RootView_h */

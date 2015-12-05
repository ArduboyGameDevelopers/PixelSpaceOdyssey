//
//  TileView.h
//  PixelSpaceOdyssey
//
//  Created by Alex Lementuev on 11/29/15.
//  Copyright © 2015 Space Madness. All rights reserved.
//

#ifndef TileView_h
#define TileView_h

#include "View.h"
#include "TileSet.h"

class TileView : public View
{
private:
    static const int kBorder = 2;
    
private:
    TileSet* _tileSet;
    int _selectedIndex;
    
public:
    TileView(TileSet* tileSet, int width, int height);
    
public:
    virtual void render(SDL_Renderer* render) const;
    virtual void mouseDown(int x, int y);
    
public:
    inline int selectedIndex() const { return _selectedIndex; }
    
private:
    int tileIndexFromCords(int x, int y) const;
};

#endif /* TileView_h */

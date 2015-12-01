//
//  EditorView.h
//  PixelSpaceOdyssey
//
//  Created by Alex Lementuev on 11/30/15.
//  Copyright © 2015 Space Madness. All rights reserved.
//

#ifndef EditorView_h
#define EditorView_h

#include "View.h"
#include "TileSet.h"

#include "tilemap.h"

class EditorView : public View
{
private:
    TileMap* _tileMap;
    TileSet* _tileSet;
    
public:
    EditorView(TileMap* tileMap, TileSet* tileSet, int width, int height);
    
public:
    virtual void render(SDL_Renderer* render) const;
    
    virtual void mouseDown(int x, int y);
    virtual void mouseMove(int x, int y);
    virtual void mouseUp(int x, int y);
};

#endif /* EditorView_h */

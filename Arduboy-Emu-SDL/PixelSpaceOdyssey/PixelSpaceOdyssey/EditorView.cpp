//
//  EditorView.c
//  PixelSpaceOdyssey
//
//  Created by Alex Lementuev on 11/30/15.
//  Copyright © 2015 Space Madness. All rights reserved.
//

#include "EditorView.h"
#include "game.h"

EditorView::EditorView(TileMap* tileMap, TileSet* tileSet, int width, int height) :
    View(width, height),
    _tileMap(tileMap),
    _tileSet(tileSet)
{
}

void EditorView::render(SDL_Renderer* render) const
{
    const uint8_t* indices = _tileMap->indices;
    uint8_t rows = _tileMap->rows;
    uint8_t cols = _tileMap->cols;
    
    int tileWidth = _tileSet->tileWidth();
    int tileHeight = _tileSet->tileHeight();
    
    uint16_t index = 0;
    for (int i = 0; i < rows; ++i)
    {
        int drawY = top() + i * tileHeight;
        for (int j = 0; j < cols; ++j)
        {
            uint8_t tileIndex = indices[index];
            if (tileIndex > 0)
            {
                int drawX = left() + j * tileWidth;
                _tileSet->drawTile(render, tileIndex - 1, drawX, drawY);
            }
            ++index;
        }
    }
}

void EditorView::mouseDown(int x, int y)
{
    
}

void EditorView::mouseMove(int x, int y)
{
    
}

void EditorView::mouseUp(int x, int y)
{
    
}
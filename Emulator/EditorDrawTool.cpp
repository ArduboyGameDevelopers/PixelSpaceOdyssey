#include "EditorDrawTool.h"

#include "DisplayWidget.h"
#include "EditorState.h"
#include "drawing.h"
#include "game.h"

static int gridIndexFromCords(int x, int y);

EditorDrawTool::EditorDrawTool(DisplayWidget *displayWidget) :
    EditorTool(displayWidget),
    _color(128, 128, 128, 128),
    _tileX(0),
    _tileY(0)
{
}

void EditorDrawTool::paint(QPainter *painter)
{
    if (hasMouse() && editorState.tileIndex != -1)
    {
        QRect rect(_tileX, _tileY, GRID_CELL_WIDTH_PX, GRID_CELL_HEIGHT_PX);
        painter->drawRect(rect);
    }
}

void EditorDrawTool::onMousePressed(int x, int y)
{
    if (editorState.tileIndex != -1)
    {
        int gridIndex = gridIndexFromCords(x, y);
        tileMap.indices[gridIndex] = editorState.tileIndex;
    }
}

void EditorDrawTool::onMouseMoved(int x, int y)
{
    if (editorState.tileIndex != -1)
    {
        int col = (x / PIXEL_WIDTH - drawTransX) / GRID_CELL_WIDTH;
        int row = (y / PIXEL_HEIGHT - drawTransY) / GRID_CELL_HEIGHT;
        int offsetX = drawTransX * PIXEL_WIDTH;
        int offsetY = drawTransY * PIXEL_HEIGHT;
        
        _tileX = offsetX + col * GRID_CELL_WIDTH_PX;
        _tileY = offsetY + row * GRID_CELL_HEIGHT_PX;
        
        if (mouseDown())
        {
            int gridIndex = gridIndexFromCords(x, y);
            tileMap.indices[gridIndex] = editorState.tileIndex;
        }
    }
}

void EditorDrawTool::onMouseReleased(int x, int y)
{
}

int gridIndexFromCords(int x, int y)
{
    int col = (x / PIXEL_WIDTH - drawTransX) / GRID_CELL_WIDTH;
    int row = (y / PIXEL_HEIGHT - drawTransY) / GRID_CELL_HEIGHT;
    int width = W2S(tileMapWidth);
    
    int colCount = width / GRID_CELL_WIDTH;
    return colCount * row + col;
}
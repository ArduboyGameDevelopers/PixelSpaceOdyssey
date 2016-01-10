#include "EditorDrawTool.h"

#include "DisplayWidget.h"
#include "EditorState.h"
#include "drawing.h"
#include "game.h"
#include "Constants.h"

static int gridIndexFromCords(int x, int y);

EditorDrawTool::EditorDrawTool(DisplayWidget *displayWidget) :
    EditorTool(displayWidget),
    _color(128, 128, 128, 128)
{
}

void EditorDrawTool::onPaint(QPainter *painter)
{
    QRect rect(tileX(), tileY(), GRID_CELL_WIDTH_PX, GRID_CELL_HEIGHT_PX);
    painter->drawRect(rect);
}

void EditorDrawTool::onMousePressed(int x, int y)
{
    if (editorState.tileIndex() != -1)
    {
        int gridIndex = gridIndexFromCords(x, y);
        Level::current()->setTileMapIndex(gridIndex, editorState.tileIndex());
    }
}

void EditorDrawTool::onMouseMoved(int x, int y)
{
    int oldTileX = tileX();
    int oldTileY = tileY();

    EditorTool::onMouseMoved(x, y);
    
    if (oldTileX != tileX() || oldTileY != tileY())
    {
        displayWidget()->repaint();
    }
    
    if (mouseDown() && editorState.tileIndex() != -1)
    {
        int gridIndex = gridIndexFromCords(x, y);
        Level::current()->setTileMapIndex(gridIndex, editorState.tileIndex());
    }
}

void EditorDrawTool::onMouseReleased(int, int)
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

#include "EditorTool.h"
#include "DisplayWidget.h"

#include "game.h"
#include "drawing.h"

EditorTool::EditorTool(DisplayWidget *displayWidget) :
    _displayWidget(displayWidget),
    _hasMouse(false),
    _mouseDown(false),
    _lastMouseX(0),
    _lastMouseY(0),
    _tileX(0),
    _tileY(0)
{
}

void EditorTool::start()
{
    onStart();
}

void EditorTool::stop()
{
    _mouseDown = false;
    _hasMouse = false;
    onStop();
}

void EditorTool::paint(QPainter *painter)
{
    onPaint(painter);
}

void EditorTool::keyPressed(int key)
{
    onKeyPressed(key);
}

void EditorTool::keyReleased(int key)
{
    onKeyReleased(key);
}

void EditorTool::mousePressed(int x, int y)
{
    _mouseDown = true;
    _lastMouseX = x;
    _lastMouseY = y;

    updateTileCords(x, y);
    onMousePressed(x, y);
}

void EditorTool::mouseMoved(int x, int y)
{
    onMouseMoved(x, y);
    updateTileCords(x, y);

    _lastMouseX = x;
    _lastMouseY = y;
}

void EditorTool::mouseReleased(int x, int y)
{
    if (_mouseDown)
    {
        onMouseReleased(x, y);
        _mouseDown = false;
    }
    
    _lastMouseX = x;
    _lastMouseY = y;
}

void EditorTool::mouseEntered()
{
    _hasMouse = true;
    onMouseEntered();
}

void EditorTool::mouseExited()
{
    onMouseExited();

    _hasMouse = false;
    _mouseDown = false;
}

void EditorTool::onStart()
{
}

void EditorTool::onStop()
{
}

void EditorTool::onPaint(QPainter *)
{
}

void EditorTool::onKeyPressed(int)
{
}

void EditorTool::onKeyReleased(int)
{
}

void EditorTool::onMousePressed(int, int)
{
}

void EditorTool::onMouseMoved(int, int)
{
}

void EditorTool::onMouseReleased(int, int)
{
}

void EditorTool::onMouseEntered()
{
}

void EditorTool::onMouseExited()
{
}

void EditorTool::updateTileCords(int x, int y)
{
    int col = (x / PIXEL_WIDTH - drawTransX) / GRID_CELL_WIDTH;
    int row = (y / PIXEL_HEIGHT - drawTransY) / GRID_CELL_HEIGHT;
    int offsetX = drawTransX * PIXEL_WIDTH;
    int offsetY = drawTransY * PIXEL_HEIGHT;
    
    _tileX = offsetX + col * GRID_CELL_WIDTH_PX;
    _tileY = offsetY + row * GRID_CELL_HEIGHT_PX;
}

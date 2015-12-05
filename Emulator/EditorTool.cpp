#include "EditorTool.h"
#include "DisplayWidget.h"

EditorTool::EditorTool(DisplayWidget *displayWidget) :
    _displayWidget(displayWidget),
    _hasMouse(false),
    _mouseDown(false),
    _lastMouseX(0),
    _lastMouseY(0)
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

    onMousePressed(x, y);
}

void EditorTool::mouseMoved(int x, int y)
{
    onMouseMoved(x, y);

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

void EditorTool::onPaint(QPainter *painter)
{
}

void EditorTool::onKeyPressed(int key)
{
}

void EditorTool::onKeyReleased(int key)
{
}

void EditorTool::onMousePressed(int x, int y)
{
}

void EditorTool::onMouseMoved(int x, int y)
{
}

void EditorTool::onMouseReleased(int x, int y)
{
}

void EditorTool::onMouseEntered()
{
}

void EditorTool::onMouseExited()
{
}

#include "EditorPanTool.h"

#include "DisplayWidget.h"
#include "drawing.h"
#include "game.h"
#include <Arduino.h>

EditorPanTool::EditorPanTool(DisplayWidget *displayWidget) :
    EditorTool(displayWidget),
    _prevTool(displayWidget->currentTool())
{
}

void EditorPanTool::onStart()
{
    setCursor(Qt::OpenHandCursor);
}

void EditorPanTool::onStop()
{
    unsetCursor();
    displayWidget()->setCurrentTool(_prevTool);
}

void EditorPanTool::onMousePressed(int x, int y)
{
    setCursor(Qt::ClosedHandCursor);
}

void EditorPanTool::onMouseMoved(int x, int y)
{
    if (mouseDown())
    {
        int dx = x - lastMouseX();
        int dy = y - lastMouseY();

        camX -= S2W(dx) / PIXEL_WIDTH;
        camY -= S2W(dy) / PIXEL_HEIGHT;

        int16_t maxCamX = tileMapWidth - CAM_WIDTH_HALF;
        int16_t maxCamY = tileMapHeight - CAM_HEIGHT_HALF;
        camX = constrain(camX, CAM_WIDTH_HALF, maxCamX);
        camY = constrain(camY, CAM_HEIGHT_HALF, maxCamY);
    }
}

void EditorPanTool::onMouseReleased(int x, int y)
{
    setCursor(Qt::OpenHandCursor);
}

void EditorPanTool::setCursor(Qt::CursorShape shape)
{
    QCursor cursor(shape);
    displayWidget()->setCursor(cursor);
}

void EditorPanTool::unsetCursor()
{
    displayWidget()->unsetCursor();
}

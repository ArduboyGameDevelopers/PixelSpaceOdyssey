#ifndef EDITORTOOL_H
#define EDITORTOOL_H

#include "Object.h"

#include <QtGlobal>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QPainter>

class DisplayWidget;

class EditorTool : public Object
{
    OBJECT_DEF(EditorTool)

public:
    EditorTool(DisplayWidget *displayWidget);
    
public:
    void paint(QPainter *painter);
    
    void start();
    void stop();

    void keyPressed(int key);
    void keyReleased(int key);

    void mousePressed(int x, int y);
    void mouseMoved(int x, int y);
    void mouseReleased(int x, int y);

    void mouseEntered();
    void mouseExited();

protected:
    virtual void onStart();
    virtual void onStop();
    
    virtual void onPaint(QPainter *painter);

    virtual void onKeyPressed(int key);
    virtual void onKeyReleased(int key);

    virtual void onMousePressed(int x, int y);
    virtual void onMouseMoved(int x, int y);
    virtual void onMouseReleased(int x, int y);

    virtual void onMouseEntered();
    virtual void onMouseExited();

protected:
    inline DisplayWidget* displayWidget() const { return _displayWidget; }
    inline bool hasMouse() const { return _hasMouse; }
    inline bool mouseDown() const { return _mouseDown; }
    inline int lastMouseX() const { return _lastMouseX; }
    inline int lastMouseY() const { return _lastMouseY; }
    inline int tileX() const { return _tileX; }
    inline int tileY() const { return _tileY; }
    
private:
    void updateTileCords(int x, int y);

private:
    DisplayWidget* _displayWidget;
    bool _hasMouse;
    bool _mouseDown;
    int _lastMouseX;
    int _lastMouseY;
    int _tileX;
    int _tileY;
};

#endif // EDITORTOOL_H

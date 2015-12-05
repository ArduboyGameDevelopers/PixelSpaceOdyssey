#ifndef EDITORDRAWTOOL_H
#define EDITORDRAWTOOL_H

#include "EditorTool.h"

class DisplayWidget;

class EditorDrawTool : public EditorTool
{
public:
    EditorDrawTool(DisplayWidget *displayWidget);

protected:
    void paint(QPainter *painter) Q_DECL_OVERRIDE;
    
    void onMousePressed(int x, int y) Q_DECL_OVERRIDE;
    void onMouseMoved(int x, int y) Q_DECL_OVERRIDE;
    void onMouseReleased(int x, int y) Q_DECL_OVERRIDE;
    
private:
    QColor _color;
    int _tileX;
    int _tileY;
};

#endif // EDITORDRAWTOOL_H

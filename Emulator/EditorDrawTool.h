#ifndef EDITORDRAWTOOL_H
#define EDITORDRAWTOOL_H

#include "EditorTool.h"

class DisplayWidget;

class EditorDrawTool : public EditorTool
{
public:
    EditorDrawTool(DisplayWidget *displayWidget);

protected:
    void onMousePressed(int x, int y) Q_DECL_OVERRIDE;
    void onMouseMoved(int x, int y) Q_DECL_OVERRIDE;
    void onMouseReleased(int x, int y) Q_DECL_OVERRIDE;
};

#endif // EDITORDRAWTOOL_H

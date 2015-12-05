#ifndef EDITORPANTOOL_H
#define EDITORPANTOOL_H

#include "EditorTool.h"

class EditorPanTool : public EditorTool
{
public:
    EditorPanTool(DisplayWidget* displayWidget);

protected:
    void onStart() Q_DECL_OVERRIDE;
    void onStop() Q_DECL_OVERRIDE;
    void onMousePressed(int x, int y) Q_DECL_OVERRIDE;
    void onMouseMoved(int x, int y) Q_DECL_OVERRIDE;
    void onMouseReleased(int x, int y) Q_DECL_OVERRIDE;
    
private:
    void setCursor(Qt::CursorShape shape);
    void unsetCursor();
};

#endif // EDITORPANTOOL_H

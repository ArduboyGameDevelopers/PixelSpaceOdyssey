#ifndef EDITORCHARTOOL_H
#define EDITORCHARTOOL_H

#include "Level.h"
#include "EditorTool.h"

#include <QImage>

class EditorCharTool : public EditorTool
{
public:
    EditorCharTool(DisplayWidget *displayWidget, CharacterType characterType);
    
protected:
    void onPaint(QPainter *painter);
    void onMousePressed(int x, int y);
    void onMouseMoved(int x, int y);

public:
    inline CharacterType characterType() const { return _characterType; }

private:
    CharacterType _characterType;
    QImage _characterImage;
};

#endif // EDITORCHARTOOL_H

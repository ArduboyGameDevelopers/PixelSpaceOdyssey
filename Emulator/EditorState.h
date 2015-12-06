#ifndef EDITORSTATE_H
#define EDITORSTATE_H

#include "Level.h"

class EditorState
{
public:
    EditorState();

public:
    inline int tileIndex() const { return _tileIndex; }
    inline void setTileIndex(int tileIndex) { _tileIndex = tileIndex; }

private:
    int _tileIndex;
};

extern EditorState editorState;

#endif // EDITORSTATE_H

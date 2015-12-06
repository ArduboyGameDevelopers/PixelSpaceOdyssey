#ifndef EDITORSTATE_H
#define EDITORSTATE_H

#include "Level.h"

class TileSet;

class EditorState
{
public:
    EditorState();
    virtual ~EditorState();

public:
    inline TileSet *tileSet() const { return _tileSet; }
    void setTileSet(TileSet *tileSet);

    inline int tileIndex() const { return _tileIndex; }
    inline void setTileIndex(int tileIndex) { _tileIndex = tileIndex; }

private:
    TileSet* _tileSet;
    int _tileIndex;
};

extern EditorState editorState;

#endif // EDITORSTATE_H

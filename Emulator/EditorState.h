#ifndef EDITORSTATE_H
#define EDITORSTATE_H

#include "Level.h"

#include <QList>

class TileSet;

class EditorState
{
public:
    EditorState();
    virtual ~EditorState();

public:
    void addTileSet(TileSet *tileSet);

    inline TileSet *currentTileSet() const { return _tiles.at(_tileSetIndex); }
    void setTileSetIndex(int tileSetIndex);

    inline int tileIndex() const { return _tileIndex; }
    inline void setTileIndex(int tileIndex) { _tileIndex = tileIndex; }
    
    inline int characterIndex() const { return _characterIndex; }
    inline void setCharacterIndex(int characterIndex) { _characterIndex = characterIndex; }

private:
    QList<TileSet *> _tiles;
    int _tileSetIndex;
    int _tileIndex;
    int _characterIndex;
};

extern EditorState editorState;

#endif // EDITORSTATE_H

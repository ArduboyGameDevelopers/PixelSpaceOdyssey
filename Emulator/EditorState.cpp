#include "EditorState.h"

#include "Tileset.h"

EditorState editorState;

EditorState::EditorState() :
    _tileSet(NULL),
    _tileIndex(0)
{
}

EditorState::~EditorState()
{
    RELEASE(_tileSet);
}

void EditorState::setTileSet(TileSet *tileSet)
{
    TileSet *oldTileSet = _tileSet;
    _tileSet = RETAIN(tileSet);
    RELEASE(oldTileSet);
}

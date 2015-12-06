#include "EditorState.h"

#include "Tileset.h"
#include "game.h"

EditorState editorState;

EditorState::EditorState() :
    _tiles(),
    _tileSetIndex(-1),
    _tileIndex(0)
{
}

EditorState::~EditorState()
{
    for (int i = 0; i < _tiles.size(); ++i)
    {
        _tiles.at(i)->release();
    }
}

void EditorState::addTileSet(TileSet *tileSet)
{
    _tiles.append(tileSet->retain());
}

void EditorState::setTileSetIndex(int tileSetIndex)
{
    Q_ASSERT(tileSetIndex >= 0 && tileSetIndex < _tiles.size());
    _tileSetIndex = tileSetIndex;
    
    TileSet *tileSet = currentTileSet();
    
    PgmPtr *tiles = (PgmPtr *)malloc(tileSet->tileCount() * sizeof(PgmPtr));
    for (int i = 0; i < tileSet->tileCount(); ++i)
    {
        TileImage *tile = tileSet->getTileImage(i);
        PgmMem tileMem(tile->image());
        tiles[i] = tileMem.dataCopy();
    }
    tileMap.tiles = tiles;
}

#ifndef LEVEL_H
#define LEVEL_H

#include "tilemap.h"

#include "Object.h"
#include "Tileset.h"
#include "CharacterInfo.h"

#include <QtGlobal>
#include <QImage>

class Level : public Object
{
    OBJECT_DEF(Level)

protected:
    Level();

public:
    Level(uint8_t* indices, uint8_t rows, uint8_t cols, const QString &filename = QString());
    virtual ~Level();
    
public:
    static Level* readFromFile(const QString &filename);
    static Level* readFromImage(const QImage &image, const TileSet* tileSet);
    void writeToFile(const QString &filename);
    
public:
    static Level *current() { return _currentLevel; }
    static void setCurrent(Level *level);
    static void restart();

public:
    void addEnemy(CharacterType type, int x, int y, Direction dir);
    void deleteEnemy(int index);
    void resize(int top, int bottom, int left, int right);
    
private:
    void updateItemIndices();

public:
    inline const QString &filename() const { return _filename; }
    inline bool hasFilename() const { return _filename.length() > 0; }
    
    inline CharacterInfo player() const { return _player; }
    inline void setPlayerPos(int x, int y) { _player.setPos(x, y); }
    inline void setPlayerDir(Direction dir) { _player.setDirection(dir); }
    
    inline const QList<CharacterInfo> enemies() const { return _enemies; }
    inline int enemiesCount() const { return _enemies.size(); }
    inline void setEnemyDir(int enemyIndex, Direction dir)
    {
        assert(enemyIndex >= 0 && enemyIndex < enemiesCount());
        _enemies[enemyIndex].setDirection(dir);
    }
    
    inline uint8_t* indices() const { return _indices; }
    void setTileMapIndex(int gridIndex, uint8_t tileIndex);

    inline uint8_t tileSetId() const { return _tileSetId; }
    inline void setTileSetId(uint8_t tileSetId) { _tileSetId = tileSetId; }

    inline uint8_t rows() const { return _rows; }
    inline void setRows(uint8_t rows) { _rows = rows; }

    inline uint8_t cols() const { return _cols; }
    inline void setCols(uint8_t cols) { _cols = cols; }

protected:
    inline void setIndices(uint8_t* indices) { _indices = indices; }
    
private:
    static Level * _currentLevel;
    QString _filename;
    CharacterInfo _player;
    QList<CharacterInfo> _enemies;
    uint8_t  _tileSetId;
    uint8_t* _indices;
    uint8_t  _rows;
    uint8_t  _cols;
};

#endif // LEVEL_H

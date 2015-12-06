#include <string.h>

#include "Level.h"
#include "Tileset.h"
#include "Constants.h"
#include "game.h"

#include <QtGlobal>
#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QTextStream>
#include <QDebug>

static const uint8_t kVersion = 1;

Level* Level::_currentLevel(NULL);

Level::Level() :
    _tileSetId(0),
    _indices(NULL),
    _rows(0),
    _cols(0),
    _playerX(0),
    _playerY(0)
{
}

Level::Level(uint8_t* indices, uint8_t rows, uint8_t cols) :
    _tileSetId(0),
    _rows(rows),
    _cols(cols),
    _playerX(0),
    _playerY(0)
{
    size_t size = rows * cols * sizeof(uint8_t);
    _indices = (uint8_t *) malloc(size);
    memcpy(_indices, indices, size);
}

Level::~Level()
{
    if (_indices)
    {
        free(_indices);
    }
}

Level* Level::readFromFile(const QString &filename)
{
    QFile file(filename);
    if (file.open(QIODevice::ReadOnly))
    {
        QByteArray levelData = file.readAll();
        file.close();
        
        QJsonDocument levelDoc = QJsonDocument::fromJson(levelData);
        QJsonObject levelJson = levelDoc.object();
        
        QJsonObject player = levelJson["player"].toObject();
        int playerX = player["x"].toInt();
        int playerY = player["y"].toInt();
        
        QJsonObject map = levelJson["map"].toObject();
        int cols = map["cols"].toInt();
        int rows = map["rows"].toInt();
        int indexCount = rows * cols;
        uint8_t indices[indexCount];
        
        QString indicesString = map["indices"].toString();
        QStringList tokens = indicesString.split(",");
        for (int i = 0; i < indexCount; ++i)
        {
            indices[i] = tokens.at(i).toInt();
        }
        
        Level *level = new Level(indices, rows, cols);
        level->setPlayerX(playerX);
        level->setPlayerY(playerY);
        return level;
    }
    
    return NULL;
}

Level *Level::readFromImage(const QImage &image, const TileSet *tileSet)
{
    int tileWidth = tileSet->tileWidth();
    int tileHeight = tileSet->tileHeight();

    int rows = image.height() / tileWidth;
    int cols = image.width() / tileHeight;

    uint8_t indices[rows * cols];
    int nextIndex = 0;
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            int x = j * tileWidth;
            int y = i * tileHeight;

            TileImage tileImage(image, x, y, tileWidth, tileHeight);
            indices[nextIndex++] = tileSet->indexOfTile(&tileImage);
        }
    }

    return new Level(indices, rows, cols);
}

void Level::writeToFile(const QString &filename)
{
    QJsonObject json;
    
    // version
    json["version"] = kVersion;
    
    // player
    QJsonObject player;
    player["x"] = _playerX;
    player["y"] = _playerY;
    player["direction"] = 1;
    json["player"] = player;
    
    // map
    QJsonObject map;
    map["cols"] = _cols;
    map["rows"] = _rows;

    // indices
    QStringList indicesList;
    int indexCount = _rows * _cols;
    for (int i = 0; i < indexCount; ++i)
    {
        indicesList << QString::number(_indices[i]);
    }
    map["indices"] = indicesList.join(",");
    
    json["map"] = map;
    
    QFile file(filename);
    if (file.open(QIODevice::WriteOnly))
    {
        QJsonDocument saveDoc(json);
        file.write(saveDoc.toJson());
        file.close();
    }
}

void Level::setCurrent(Level *level)
{
    Level *oldLevel = _currentLevel;
    _currentLevel = RETAIN(level);
    RELEASE(oldLevel);
    
    int cols = level->cols();
    int rows = level->rows();
    
    player.x = S2W(level->playerX());
    player.y = S2W(level->playerY());
    // player.dir = level->direction();
    
    tileMap.indices = level->indices();
    tileMap.rows = rows;
    tileMap.cols = cols;
    
    tileMapWidth = S2W(cols * GRID_CELL_WIDTH);
    tileMapHeight = S2W(rows * GRID_CELL_HEIGHT);

    camX = CAM_WIDTH_HALF;
    camY = CAM_WIDTH_HALF;
}

void Level::resize(uint8_t rows, uint8_t cols)
{
    if (rows == _rows && cols == _cols) return;

    int size = rows * cols * sizeof(uint8_t);
    uint8_t *newIndices = (uint8_t *)malloc(size);
    memset(newIndices, 0, size);

    uint8_t r = qMin(rows, _rows);
    uint8_t c = qMin(cols, _cols);

    for (int i = 0; i < r; ++i)
    {
        for (int j = 0; j < c; ++j)
        {
            int from = i * _cols + j;
            int to = i * c + j;
            newIndices[to] = _indices[from];
        }
    }

    _rows = rows;
    _cols = cols;

    free(_indices);
    _indices = newIndices;

    setCurrent(this);
}

#include <string.h>

#include "Level.h"
#include "Constants.h"
#include "game.h"

#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QTextStream>
#include <QDebug>

static const uint8_t kVersion = 1;

static QStringList readLines(const QString &filename);

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

void setGameLevel(Level *level)
{
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
}
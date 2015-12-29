#include <string.h>

#include "Level.h"
#include "Tileset.h"
#include "Constants.h"
#include "game.h"
#include "MainWindow.h"

#include <QtGlobal>
#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QTextStream>
#include <QDebug>

#include "enemies.h"

static const uint8_t kVersion = 1;

Level* Level::_currentLevel(NULL);

Level::Level() :
    _filename(),
    _player(CharacterTypePlayer, 0, 0, DIR_RIGHT),
    _enemies(),
    _tileSetId(0),
    _indices(NULL),
    _rows(0),
    _cols(0)
{
}

Level::Level(uint8_t* indices, uint8_t rows, uint8_t cols, const QString &filename) :
    _filename(filename),
    _player(CharacterTypePlayer, 0, 0, DIR_RIGHT),
    _enemies(),
    _tileSetId(0),
    _rows(rows),
    _cols(cols)
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
        QJsonObject levelObj = levelDoc.object();
        
        QJsonObject playerObj = levelObj["player"].toObject();
        int playerX = playerObj["x"].toInt();
        int playerY = playerObj["y"].toInt();
        Direction characterDir = (Direction) playerObj["direction"].toInt();
        
        QJsonObject mapObj = levelObj["map"].toObject();
        int cols = mapObj["cols"].toInt();
        int rows = mapObj["rows"].toInt();
        int indexCount = rows * cols;
        uint8_t indices[indexCount];
        
        QString indicesString = mapObj["indices"].toString();
        QStringList tokens = indicesString.split(",");
        for (int i = 0; i < indexCount; ++i)
        {
            indices[i] = tokens.at(i).toInt();
        }
        
        Level *level = new Level(indices, rows, cols, filename);
        level->setPlayerPos(playerX, playerY);
        level->setPlayerDir(characterDir);
        
        QJsonArray enemies = levelObj["enemies"].toArray();
        for (int i = 0; i < enemies.size(); ++i)
        {
            QJsonObject enemyObj = enemies.at(i).toObject();
            QString name = enemyObj["name"].toString();
            CharacterType characterType = CharacterInfo::typeFromName(name);
            if (characterType == CharacterTypeCount)
            {
                qDebug() << "Unknown enemy type: " << name;
                continue;
            }
            
            int x = enemyObj["x"].toInt();
            int y = enemyObj["y"].toInt();
            Direction enemyDir = (Direction) enemyObj["direction"].toInt();
            
            level->addEnemy(characterType, x, y, enemyDir);
        }
        
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
            indices[nextIndex++] = tileSet->indexOfTile(&tileImage) + 1;
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
    QJsonObject playerObj;
    playerObj["x"] = _player.x();
    playerObj["y"] = _player.y();
    playerObj["direction"] = (int) _player.direction();
    json["player"] = playerObj;
    
    // enemies
    QJsonArray enemiesObj;
    for (int i = 0; i < _enemies.size(); ++i)
    {
        const CharacterInfo &enemy = _enemies.at(i);
        
        QJsonObject enemyObj;
        enemyObj["name"] = enemy.name();
        enemyObj["x"] = enemy.x();
        enemyObj["y"] = enemy.y();
        enemyObj["direction"] = (int) enemy.direction();
        
        enemiesObj << enemyObj;
    }
    json["enemies"] = enemiesObj;
    
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
    
    _filename = filename;
}

void Level::setCurrent(Level *level)
{
    Level *oldLevel = _currentLevel;
    _currentLevel = RETAIN(level);
    RELEASE(oldLevel);
    
    int cols = level->cols();
    int rows = level->rows();

    const CharacterInfo &player = level->player();
    
    ::player.x = S2W(player.x());
    ::player.y = S2W(player.y());
    ::player.dir = (int) player.direction();
    
    tileMap.indices = level->indices();
    tileMap.rows = rows;
    tileMap.cols = cols;
    
    tileMapWidth = S2W(cols * GRID_CELL_WIDTH);
    tileMapHeight = S2W(rows * GRID_CELL_HEIGHT);

    camX = CAM_WIDTH_HALF;
    camY = CAM_WIDTH_HALF;
    
    // setup enemies
    ::initEnemies(level->enemiesCount());
    for (int i = 0; i < level->enemiesCount(); ++i)
    {
        const CharacterInfo &enemyInfo = level->enemies().at(i);
        switch (enemyInfo.type())
        {
            case CharacterTypeBear:
            {
                break;
            }
            case CharacterTypeDog:
            {
                Character enemy = EnemyMakeDog();
                enemy.dir = enemyInfo.direction();
                ::addEnemy(enemy, S2W(enemyInfo.x()), S2W(enemyInfo.y()));
                break;
            }
            case CharacterTypeSpiderSmall:
            {
                break;
            }
            case CharacterTypeSpiderLarge:
            {
                Character enemy = EnemyMakeSpiderLargeCharacter();
                enemy.dir = enemyInfo.direction();
                ::addEnemy(enemy, S2W(enemyInfo.x()), S2W(enemyInfo.y()));
                break;
            }
        }
    }
    
    MainWindow::instance()->updateLevelUi(level);
}

void Level::restart()
{
    setCurrent(Level::current());
}

void Level::addEnemy(CharacterType type, int x, int y, Direction dir)
{
    CharacterInfo enemy(type, x, y, DIR_LEFT);
    enemy.setDirection(dir);
    _enemies.append(enemy);

    MainWindow::instance()->updateLevelUi(this);
}

void Level::deleteEnemy(int index)
{
    Q_ASSERT(index >= 0 && index < _enemies.length());
    _enemies.removeAt(index);
    
    MainWindow::instance()->updateLevelUi(this);
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

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

static const uint8_t kVersion = 3;

static void deleteEnemyCharacter(int id);

static Level* readLevelV1(QJsonObject levelObj, const QString &filename);
static Level* readLevelV2(QJsonObject levelObj, const QString &filename);
static Level* readLevelV3(QJsonObject levelObj, const QString &filename);

Level* Level::_currentLevel(NULL);

Level::Level() :
    _filename(),
    _player(new CharacterInfo(CharacterTypePlayer, 0, 0, DIR_RIGHT)),
    _enemies(),
    _tileSetId(0),
    _indices(NULL),
    _rows(0),
    _cols(0)
{
}

Level::Level(uint8_t* indices, uint8_t rows, uint8_t cols, const QString &filename) :
    _filename(filename),
    _player(new CharacterInfo(CharacterTypePlayer, 0, 0, DIR_RIGHT)),
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
    
    for (int i = 0; i < _enemies.size(); ++i)
    {
        delete _enemies[i];
    }
    _enemies.clear();
    
    delete _player;
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
        
        int version = levelObj["version"].toInt();
        switch (version)
        {
            case 1:
                return readLevelV1(levelObj, filename);
            case 2:
                return readLevelV2(levelObj, filename);
            case 3:
                return readLevelV3(levelObj, filename);
        }
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
    playerObj["x"] = _player->x();
    playerObj["y"] = _player->y();
    playerObj["direction"] = (int) _player->direction();
    json["player"] = playerObj;
    
    // enemies
    QJsonArray enemiesObj;
    for (int i = 0; i < _enemies.size(); ++i)
    {
        const CharacterInfo *enemy = _enemies.at(i);
        
        QJsonObject enemyObj;
        enemyObj["name"] = enemy->name();
        enemyObj["x"] = enemy->x();
        enemyObj["y"] = enemy->y();
        enemyObj["initialBehavior"] = CharacterInitialBehaviorGetName(enemy->initialBehavior());
        enemyObj["patrolBehavior"] = CharacterPatrolBehaviorGetName(enemy->patrolBehavior());
        enemyObj["direction"] = (int) enemy->direction();
        
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
    
    emulator.reset();

    const CharacterInfo *player = level->player();

    initGame();
    
    ::player.x = S2W(player->x());
    ::player.y = S2W(player->y());
    ::player.dir = (int) player->direction();
    
    tileMap.indices = level->indices();
    tileMap.rows = rows;
    tileMap.cols = cols;
    tileMap.items = 0;
    
    tileMapWidth = S2W(cols * GRID_CELL_WIDTH);
    tileMapHeight = S2W(rows * GRID_CELL_HEIGHT);

    camX = CAM_WIDTH_HALF;
    camY = CAM_WIDTH_HALF;
    
    // setup enemies
    ::initEnemies(level->enemiesCount());
    
    for (int i = 0; i < level->enemiesCount(); ++i)
    {
        const CharacterInfo *enemyInfo = level->enemies().at(i);
        
        Character enemy;
        switch (enemyInfo->type())
        {
            case CharacterTypeBear:
            {
                enemy = EnemyMakeBearCharacter();
                break;
            }
            case CharacterTypeDog:
            {
                enemy = EnemyMakeDog();
                break;
            }
            case CharacterTypeSpiderSmall:
            {
                break;
            }
            case CharacterTypeSpiderLarge:
            {
                enemy = EnemyMakeSpiderLarge();
                break;
            }
        }
        
        enemy.dir = enemyInfo->direction();
        enemy.id = enemyInfo->id();
        ::addEnemy(enemy, S2W(enemyInfo->x()), S2W(enemyInfo->y()));
    }
    
    MainWindow::instance()->updateLevelUi(level);
}

void Level::restart()
{
    setCurrent(Level::current());
}

CharacterInfo *Level::addEnemy(CharacterType type, int x, int y, Direction dir)
{
    CharacterInfo *enemy = new CharacterInfo(type, x, y, dir);
    _enemies.append(enemy);

    MainWindow::instance()->updateLevelUi(this);
    
    return enemy;
}

void Level::deleteEnemy(int index)
{
    const CharacterInfo *enemy = _enemies.at(index);
    deleteEnemyCharacter(enemy->id());
    
    Q_ASSERT(index >= 0 && index < _enemies.length());
    _enemies.removeAt(index);
    
    MainWindow::instance()->updateLevelUi(this);
}

void Level::resize(int top, int bottom, int left, int right)
{
    uint8_t cols = left + right + _cols;
    uint8_t rows = top + bottom + _rows;
    int size = cols * rows * sizeof(uint8_t);
    uint8_t *newIndices = (uint8_t *)malloc(size);
    memset(newIndices, 0, size);

    int fromRow = 0, fromCol = 0, toRow = 0, toCol = 0;
    if (top > 0)
    {
        fromRow = 0;
        toRow = top;
    }
    else if (top < 0)
    {
        fromRow = -top;
        toRow = 0;
    }

    if (left > 0)
    {
        fromCol = 0;
        toCol = left;
    }
    else
    {
        fromCol = -left;
        toCol = 0;
    }

    int r = qMin(rows, _rows);
    int c = qMin(cols, _cols);
    for (int i = 0; i < r; ++i)
    {
        for (int j = 0; j < c; ++j)
        {
            int from = (fromRow + i) * _cols + (fromCol + j);
            int to = (toRow + i) * cols + (toCol + j);
            newIndices[to] = _indices[from];
        }
    }

    _rows = rows;
    _cols = cols;

    free(_indices);
    _indices = newIndices;
    
    // move characters
    _player->setX(_player->x() + left * TILE_WIDTH_PX);
    _player->setY(_player->y() + top * TILE_HEIGHT_PX);
    
    for (int i = 0; i < enemiesCount(); ++i)
    {
        CharacterInfo *enemy = _enemies[i];
        enemy->setX(enemy->x() + left * TILE_WIDTH_PX);
        enemy->setY(enemy->y() + top * TILE_HEIGHT_PX);
    }

    setCurrent(this);
}

#pragma mark -
#pragma mark Indices

void Level::setTileMapIndex(int gridIndex, uint8_t tileIndex)
{
    if (tileIndex >= TILE_ITEM_MIN && tileIndex <= TILE_ITEM_MAX)
    {
        tileIndex = TileItemSetType(0x80, tileIndex - TILE_ITEM_MIN);
    }
    
    int oldTileIndex = tileMap.indices[gridIndex];
    tileMap.indices[gridIndex] = tileIndex;
    
    if (TILE_IS_ITEM(oldTileIndex) || TILE_IS_ITEM(tileIndex))
    {
        updateItemIndices();
    }
}

#pragma mark -
#pragma mark Items

void Level::updateItemIndices()
{
    tileMap.items = 0;
    
    uint8_t *indices = tileMap.indices;
    int indexCount = TileMapGetIndexCount(tileMap);
    int itemCount = 0;
    for (int i = 0; i < indexCount; ++i)
    {
        if (TILE_IS_ITEM(indices[i]))
        {
            indices[i] = TileItemSetIndex(indices[i], itemCount);
            ++itemCount;
        }
    }
}

#pragma mark -
#pragma mark Helpers

static int findEnemyCharacter(int id)
{
    for (int i = 0; i < enemiesCount; ++i)
    {
        if (enemies[i].id == id)
        {
            return i;
        }
    }
    
    return -1;
}

static void deleteEnemyCharacter(int id)
{
    int index = findEnemyCharacter(id);
    if (index != -1)
    {
        for (int i = index + 1; i < enemiesCount; ++i)
        {
            enemies[i - 1] = enemies[i];
        }
        --enemiesCount;
    }
}

#pragma mark -
#pragma mark Level reader

static Level* readLevelV1(QJsonObject levelObj, const QString &filename)
{
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
    
    // transform indices
    int kV1IndexLookup[] = {
        0, /* 0 */
        1, /* 1 */
        2, /* 2 */
        3, /* 3 */
        4, /* 4 */
        5, /* 5 */
        6, /* 6 */
        7, /* 7 */
        8, /* 8 */
        9, /* 9 */
        10, /* 10 */
        11, /* 11 */
        12, /* 12 */
        13, /* 13 */
        27, /* 14 */
        29, /* 15 */
        31, /* 16 */
        32, /* 17 */
        33, /* 18 */
        34, /* 19 */
        35, /* 20 */
        36, /* 21 */
        37, /* 22 */
        38, /* 23 */
        48, /* 24 */
        49, /* 25 */
        50, /* 26 */
        51, /* 27 */
        52, /* 28 */
        53, /* 29 */
        54, /* 30 */
        55, /* 31 */
        62, /* 32 */
        63, /* 33 */
        64, /* 34 */
    };
    
    for (int i = 0; i < indexCount; ++i)
    {
        indices[i] = kV1IndexLookup[indices[i]];
    }
    
    Level *level = new Level(indices, rows, cols, filename);
    level->player()->setPos(playerX, playerY);
    level->player()->setDirection(characterDir);
    
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

static Level* readLevelV2(QJsonObject levelObj, const QString &filename)
{
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
    level->player()->setPos(playerX, playerY);
    level->player()->setDirection(characterDir);
    
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

static Level* readLevelV3(QJsonObject levelObj, const QString &filename)
{
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
    level->player()->setPos(playerX, playerY);
    level->player()->setDirection(characterDir);
    
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
        
        CharacterInfo *enemy = level->addEnemy(characterType, x, y, enemyDir);
        enemy->setInitialBehavior(CharacterInitialBehaviorFromName(enemyObj["initialBehavior"].toString()));
        enemy->setPatrolBehavior(CharacterPatrolBehaviorFromName(enemyObj["patrolBehavior"].toString()));
    }
    
    return level;
}

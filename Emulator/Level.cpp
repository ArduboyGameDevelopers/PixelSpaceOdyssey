#include <string.h>

#include "Level.h"

#include <QFile>
#include <QTextStream>

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
    QStringList lines = readLines(filename);

    int lineIndex = 0;
    int version = lines.at(lineIndex++).toInt();

    int playerX, playerY;
    QString line = lines.at(lineIndex++);
    QTextStream(&line) >> playerX >> playerY;

    int rows, cols;
    line = lines.at(lineIndex++);
    QTextStream(&line) >> rows >> cols;

    uint8_t indices[rows * cols];
    for (int i = 0, index = 0; i < rows; ++i)
    {
        line = lines.at(lineIndex++);

        QStringList tokens = line.split(" ", QString::SkipEmptyParts);
        Q_ASSERT(tokens.length() == cols);
        for (int j = 0; j < cols; ++j)
        {
            indices[index++] = tokens.at(j).toInt();
        }
    }

    Level* level = new Level(indices, rows, cols);
    level->setPlayerX(playerX);
    level->setPlayerY(playerY);
    return level;
}

void Level::writeToFile(const QString &filename)
{
    QFile file(filename);
    if (file.open(QIODevice::WriteOnly))
    {
        QTextStream stream(&file);

        // version
        stream << "# version" << endl;
        stream << kVersion << endl;

        // player pos
        stream << "# player pos" << endl;
        stream << _playerX << " " << _playerY << endl;

        // indices
        stream << "# rows & cols" << endl;
        stream << _rows << " " << _cols;

        // indices data
        stream << "# indices" << endl;
        for (int i = 0, index = 0; i < _rows; ++i)
        {
            for (int j = 0; j < _cols; ++j)
            {
                stream << _indices[index++] << " ";
            }
            stream << endl;
        }

        file.close();
    }
}

QStringList readLines(const QString &filename)
{
    QStringList list;

    QFile file(filename);
    if (file.open(QIODevice::ReadOnly))
    {
        QTextStream stream(&file);

        while (!stream.atEnd())
        {
            QString line = stream.readLine().trimmed();
            if (line.length() > 0 && !line.startsWith("#"))
            {
                list << line;
            }
        }

        file.close();
    }

    return list;
}

#ifndef TILESET_H
#define TILESET_H

#include "Object.h"

#include <QImage>
#include <QPainter>

class TileSet : public Object
{
    OBJECT_DEF(TileSet)

public:
    TileSet(QImage image);

public:
    void drawTile(QPainter *painter, int tileIndex, int x, int y);

public:
    inline int tileCount() const { return _tileCount; }
    inline int tileWidth() const { return _tileWidth; }
    inline int tileHeight() const { return _tileHeight; }

private:
    QImage _image;
    int _tileWidth;
    int _tileHeight;
    int _tileCount;

};

#endif // TILESET_H

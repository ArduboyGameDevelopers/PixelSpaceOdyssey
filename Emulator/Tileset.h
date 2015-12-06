#ifndef TILESET_H
#define TILESET_H

#include "Object.h"

#include <QImage>
#include <QPainter>

#include "common.h"

class PgmMem : public Object
{
    OBJECT_DEF(PgmMem)
    
public:
    PgmMem(const QImage &image);
    virtual ~PgmMem();
    
public:
    PgmPtr dataCopy() const;

    inline PgmPtr data() const { return _data; }
    inline int width() const   { return _width; }
    inline int height() const  { return _height; }
    
private:
    PgmPtr _data;
    int    _width;
    int    _height;
};

class TileImage
{
public:
    TileImage(const QImage &image);
    TileImage(const QImage &image, int x, int y, int w, int h);

public:
    bool matches(const TileImage *other);

public:
    inline const QImage image() const { return _image; }
    inline int width() const { return _width; }
    inline int height() const { return _height; }

private:
    QImage _image;
    int    _width;
    int    _height;
};

class TileSet : public Object
{
    OBJECT_DEF(TileSet)

public:
    TileSet(const QString &filename);
    TileSet(const QString &name, const QImage &image);
    TileSet(const TileSet &other, int scale = 1);
    virtual ~TileSet();

public:
    void drawTile(QPainter *painter, int tileIndex, int x, int y);

public:
    int indexOfTile(TileImage *tileImage) const;

public:
    inline const QString name() const { return _name; }
    inline int tileCount() const { return _tileCount; }
    inline int tileWidth() const { return _tileWidth; }
    inline int tileHeight() const { return _tileHeight; }
    inline TileImage *getTileImage(int tileIndex) const
    {
        Q_ASSERT(tileIndex >= 0 && tileIndex < _tileCount);
        return _images[tileIndex];
    }

private:
    void init(const QImage &image);
    
private:
    QString _name;
    TileImage** _images;
    int _tileWidth;
    int _tileHeight;
    int _tileCount;
};

#endif // TILESET_H

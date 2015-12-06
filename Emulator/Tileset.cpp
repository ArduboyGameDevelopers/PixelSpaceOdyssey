#include "Tileset.h"

TileSet::TileSet(QImage image) :
    _images(NULL)
{
    _tileHeight = image.height();
    _tileWidth = image.height();
    _tileCount = image.width() / image.height();

    _images = (TileImage **)malloc(_tileCount * sizeof(TileImage *));
    
    for (int i = 0; i < _tileCount; ++i)
    {
        int x = i * _tileWidth;
        int y = 0;
        _images[i] = new TileImage(image, x, y, _tileWidth, _tileHeight);
    }
}

TileSet::TileSet(const TileSet &other, int scale)
{
    _tileWidth = scale * other.tileWidth();
    _tileHeight = scale * other.tileHeight();
    _tileCount = other.tileCount();

    _images = (TileImage **)malloc(_tileCount * sizeof(TileImage *));
    for (int i = 0; i < _tileCount; ++i)
    {
        QImage tileImage = other.getTileImage(i)->image().scaled(_tileWidth, _tileHeight);
        _images[i] = new TileImage(tileImage);
    }
}

TileSet::~TileSet()
{
    for (int i = 0; i < _tileCount; ++i)
    {
        delete _images[i];
    }
    free(_images);
}

void TileSet::drawTile(QPainter *painter, int tileIndex, int x, int y)
{
    Q_ASSERT(tileIndex >= 0 && tileIndex < _tileCount);

    painter->drawImage(x, y, _images[tileIndex]->image());
}

int TileSet::indexOfTile(TileImage *tileImage) const
{
    for (int tileIndex = 0; tileIndex < tileCount(); ++tileIndex)
    {
        if (_images[tileIndex]->matches(tileImage))
        {
            return tileIndex;
        }
    }

    return -1;
}

TileImage::TileImage(const QImage &image) :
    _image(image),
    _width(image.width()),
    _height(image.height())
{
}

TileImage::TileImage(const QImage &image, int x, int y, int w, int h) :
    _image(image.copy(x, y, w, h)),
    _width(w),
    _height(h)
{
}

bool TileImage::matches(const TileImage *other)
{
    return image() != other->image();
}

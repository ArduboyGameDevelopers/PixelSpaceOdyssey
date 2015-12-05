#include "Tileset.h"

TileSet::TileSet(QImage image) :
    _image(image)
{
    _tileHeight = image.height();
    _tileWidth = image.height();
    _tileCount = image.width() / image.height();
}

void TileSet::drawTile(QPainter *painter, int tileIndex, int x, int y)
{
    Q_ASSERT(tileIndex >= 0 && tileIndex < _tileCount);

    int sx = tileIndex * _tileWidth;
    int sy = 0;
    painter->drawImage(x, y, _image, sx, sy, _tileWidth, _tileHeight);
}

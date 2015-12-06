#include "Tileset.h"

#include <QFileInfo>

TileSet::TileSet(const QString &filename) :
    _name(QFileInfo(filename).baseName()),
    _images(NULL)

{
    QImage image(filename);
    init(image);
}

TileSet::TileSet(const QString &name, const QImage &image) :
    _name(name),
    _images(NULL)
{
    init(image);
}

TileSet::TileSet(const TileSet &other, int scale)
{
    _tileWidth = scale * other.tileWidth();
    _tileHeight = scale * other.tileHeight();
    _tileCount = other.tileCount();
    _name = other.name();

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

void TileSet::init(const QImage &image)
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

inline static bool isWhite(QRgb pixel)
{
    int r = qRed(pixel);
    int g = qGreen(pixel);
    int b = qBlue(pixel);
    
    return r > 250 && g > 250 & b > 250;
}

bool TileImage::matches(const TileImage *other)
{
    const QImage &image1 = image();
    const QImage &image2 = other->image();

    int width = image1.width();
    int height = image1.height();
    
    if (width != image2.width() || height != image2.height())
    {
        return false;
    }
    
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            bool white1 = isWhite(image1.pixel(x, y));
            bool white2 = isWhite(image2.pixel(x, y));
            
            if (white1 ^ white2)
            {
                return false;
            }
        }
    }
    
    return true;
}

PgmMem::PgmMem(const QImage &image) :
    _data((PgmPtr) malloc(image.width() * image.height() / 8)),
    _width(image.width()),
    _height(image.height())
{
    int bits_last_page = height() % 8;
    int bytes_high = height() / 8;
    if (bits_last_page > 0)
    {
        bytes_high += 1;
    }

    int byteIndex = 0;
    for (int y_page = 0; y_page < bytes_high; ++y_page)
    {
        for (int x = 0; x < width(); ++x)
        {
            // how many bits does this line hold
            int bits = 8;

            // if we've reached the bottom there are fewer bits to load
            if (bytes_high - 1 == y_page && bits_last_page > 0)
            {
                bits = bits_last_page;
            }
            int byte = 0;
            for (int bit_height = 0; bit_height < bits; ++bit_height)
            {
                int y = y_page * 8 + bit_height;
                QRgb pixel = image.pixel(x, y);
                if (!(qRed(pixel) > 250 && qGreen(pixel) > 250 && qBlue(pixel) > 250))
                {
                    byte |= (1 << (bit_height));
                }
            }
            _data[byteIndex++] = byte;
        }
    }
}

PgmMem::~PgmMem()
{
    free(_data);
}

PgmPtr PgmMem::dataCopy() const
{
    size_t size = width() * height() / 8;
    PgmPtr data = (PgmPtr) malloc(size);
    memcpy(data, _data, size);

    return data;
}

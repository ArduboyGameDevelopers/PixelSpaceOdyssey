#include "TilesWidget.h"

#include "EditorState.h"

#include <QtGlobal>
#include <QLayout>
#include <QToolButton>
#include <QMouseEvent>

static const int kBorder = 2;

TilesWidget::TilesWidget(QWidget *parent) :
    QWidget(parent),
    _tileEntries(NULL),
    _tileSet(NULL),
    _selectedIndex(-1)
{
}

TilesWidget::~TilesWidget()
{
    RELEASE(_tileSet);

    if (_tileEntries)
    {
        free(_tileEntries);
    }
}

void TilesWidget::paintEvent(QPaintEvent *)
{
    QPainter painter;
    painter.begin(this);
    for (int i = 0; i < _tileSet->tileCount(); ++i)
    {
        const TileEntry &entry = _tileEntries[i];
        if (entry.tileIndex == _selectedIndex)
        {
            painter.fillRect(entry.x - kBorder, entry.y - kBorder,
                             _tileSet->tileWidth() + 2 * kBorder, _tileSet->tileHeight() + 2 * kBorder,
                             Qt::red);
        }

        _tileSet->drawTile(&painter, entry.tileIndex, entry.x, entry.y);
    }
    painter.end();
}

void TilesWidget::mousePressEvent(QMouseEvent *event)
{
    int x = event->x();
    int y = event->y();
    _selectedIndex = tileIndexFromCords(x, y);
    editorState.tileIndex = _selectedIndex + 1;

    repaint();
}

void TilesWidget::setTileSet(TileSet *tileSet)
{
    TileSet *oldTileSet = _tileSet;
    _tileSet = RETAIN(tileSet);
    RELEASE(oldTileSet);

    if (tileSet)
    {
        if (_tileEntries) free(_tileEntries);
        _tileEntries = (TileEntry *) malloc(tileSet->tileCount() * sizeof(TileEntry));

        // add new buttons
        int tileX = kBorder;
        int tileY = kBorder;
        int tileWidth = _tileSet->tileWidth();
        int tileHeight = _tileSet->tileHeight();
        for (int tileIndex = 0; tileIndex < tileSet->tileCount(); ++tileIndex)
        {
            TileEntry &entry = _tileEntries[tileIndex];
            entry.tileIndex = tileIndex;
            entry.x = tileX;
            entry.y = tileY;
            entry.w = tileWidth;
            entry.h = tileHeight;

            tileX += tileWidth + kBorder;
            if (tileX >= width())
            {
                tileX = kBorder;
                tileY += tileHeight + kBorder;
            }
        }
    }

    repaint();
}

int TilesWidget::tileIndexFromCords(int x, int y) const
{
    for (int i = 0; i < _tileSet->tileCount(); ++i)
    {
        const TileEntry &entry = _tileEntries[i];
        if (x >= entry.x && x <= entry.x + entry.w &&
            y >= entry.y && y <= entry.y + entry.h)
        {
            return entry.tileIndex;
        }
    }

    return -1;
}

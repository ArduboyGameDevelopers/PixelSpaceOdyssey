#ifndef TILESWIDGET_H
#define TILESWIDGET_H

#include <QWidget>
#include <QList>

#include "Tileset.h"

typedef struct _TileEntry {
    int tileIndex;
    int x;
    int y;
    int w;
    int h;
} TileEntry;

class TilesWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TilesWidget(QWidget *parent = 0);
    virtual ~TilesWidget();

protected:
    void paintEvent(QPaintEvent *) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *) Q_DECL_OVERRIDE;

public:
    void setTileSet(TileSet *tileSet);
    inline int selectedIndex() const { return _selectedIndex; }

private:
    int tileIndexFromCords(int x, int y) const;

private:
    TileEntry* _tileEntries;
    TileSet*   _tileSet;
    int        _selectedIndex;
};

#endif // TILESWIDGET_H

#include "DisplayWidget.h"

#include "drawing.h"
#include "game.h"
#include "Arduboy.h"
#include "EditorTool.h"
#include "EditorPanTool.h"

#include <QPainter>
#include <QDebug>

const int GRID_CELL_SIZE = 8;
const int PIXEL_WIDTH    = 4;
const int PIXEL_HEIGHT   = 4;

DisplayWidget::DisplayWidget(QWidget *parent) :
    QOpenGLWidget(parent),
    _pixelRects(WIDTH * HEIGHT),
    _currentTool(NULL)
{
    setAutoFillBackground(false);
}

DisplayWidget::~DisplayWidget()
{
}

void DisplayWidget::copyScreenBuffer(const unsigned char *screenBuffer, int bufferWidth, int bufferHeight)
{
    _pixelRects.clear();
    for (int y = 0; y < bufferHeight; y++)
    {
        for (int x = 0; x < bufferWidth; x++)
        {
            int row = y / 8;
            int index = row * bufferWidth + x;
            int bit = screenBuffer[index] & (1 << (y % 8));
            if (!bit)
            {
                _pixelRects.add(x * PIXEL_WIDTH, y * PIXEL_HEIGHT, PIXEL_WIDTH, PIXEL_HEIGHT);
            }
        }
    }

    repaint();
}

void DisplayWidget::startPan()
{
    EditorPanTool *panTool = new EditorPanTool(this);
    panTool->start();
    setCurrentTool(panTool);
}

void DisplayWidget::stopPan()
{
    EditorTool *panTool = currentTool();
    panTool->stop();
}

void DisplayWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter;
    painter.begin(this);
    painter.fillRect(0, 0, width(), height(), Qt::white);
    _pixelRects.paint(&painter);

    if (_currentTool)
    {
        _currentTool->paint(&painter);
    }

    painter.end();
}

void DisplayWidget::keyPressEvent(QKeyEvent *event)
{
    if (_currentTool)
    {
        _currentTool->keyPressed(event->key());
    }
}

void DisplayWidget::keyReleaseEvent(QKeyEvent *event)
{
    if (_currentTool)
    {
        _currentTool->keyReleased(event->key());
    }
}

void DisplayWidget::mousePressEvent(QMouseEvent *event)
{
    if (_currentTool)
    {
        int x = event->localPos().x();
        int y = event->localPos().y();
        _currentTool->mousePressed(x, y);
    }
}

void DisplayWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (_currentTool)
    {
        int x = event->localPos().x();
        int y = event->localPos().y();
        _currentTool->mouseMoved(x, y);
    }
}

void DisplayWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (_currentTool)
    {
        int x = event->localPos().x();
        int y = event->localPos().y();
        _currentTool->mouseReleased(x, y);
    }
}

void DisplayWidget::leaveEvent(QEvent *event)
{
    if (_currentTool)
    {
        _currentTool->mouseExited();
    }
}

void DisplayWidget::enterEvent(QEvent *event)
{
    if (_currentTool)
    {
        _currentTool->mouseEntered();
    }
}

RectList::RectList(int capacity) :
    _capacity(capacity),
    _count(0)
{
    _rects = (Rect*) malloc(capacity * sizeof(Rect));
}

RectList::~RectList()
{
    free(_rects);
}

void RectList::paint(QPainter *painter)
{
    for (int i = 0; i < count(); ++i)
    {
        painter->fillRect(_rects[i], Qt::black);
    }
}

void RectList::clear()
{
    _count = 0;
}

void RectList::add(int x, int y, int w, int h)
{
    Q_ASSERT(_count < _capacity);
    _rects[_count++] = Rect(x, y, w, h);
}

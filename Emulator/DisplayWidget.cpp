#include "DisplayWidget.h"

#include "drawing.h"
#include "game.h"
#include "Arduboy.h"
#include "EditorTool.h"
#include "EditorPanTool.h"

#include <QPainter>
#include <QDebug>

#define RECT_COLOR_PIXEL 0, 0, 0
#define RECT_COLOR_GRID  241, 245, 248

const int PIXEL_WIDTH         = 4;
const int PIXEL_HEIGHT        = 4;

const int GRID_CELL_WIDTH     = 8;
const int GRID_CELL_HEIGHT    = 8;

const int GRID_CELL_WIDTH_PX  = GRID_CELL_WIDTH * PIXEL_WIDTH;
const int GRID_CELL_HEIGHT_PX = GRID_CELL_HEIGHT * PIXEL_HEIGHT;

static const int GRID_ROWS = WIDTH / GRID_CELL_WIDTH + 2;
static const int GRID_COLS = HEIGHT / GRID_CELL_HEIGHT + 2;

DisplayWidget::DisplayWidget(QWidget *parent) :
    QOpenGLWidget(parent),
    _pixelRects(WIDTH * HEIGHT, RECT_COLOR_PIXEL),
    _gridRects(GRID_ROWS * GRID_COLS, RECT_COLOR_GRID),
    _currentTool(NULL),
    _gridVisible(false)
{
    setAutoFillBackground(false);
    
    for (int i = 0; i < GRID_COLS; ++i)
    {
        bool cellVisible = i % 2 != 0;
        for (int j = 0; j < GRID_ROWS; ++j)
        {
            if (cellVisible)
            {
                int x = j * GRID_CELL_WIDTH_PX;
                int y = i * GRID_CELL_HEIGHT_PX;
                _gridRects.add(x, y, GRID_CELL_WIDTH_PX, GRID_CELL_HEIGHT_PX);
            }
            cellVisible = !cellVisible;
        }
    }
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
    
    // background
    painter.fillRect(0, 0, width(), height(), Qt::white);
    
    // draw grid
    if (_gridVisible)
    {
        int drawOffsetX = drawTransX * PIXEL_WIDTH;
        int drawOffsetY = drawTransY * PIXEL_HEIGHT;
        int kx = 2 * GRID_CELL_WIDTH_PX;
        int ky = 2 * GRID_CELL_HEIGHT_PX;
        int offsetX = drawOffsetX - (drawOffsetX / kx) * kx;
        int offsetY = drawOffsetY - (drawOffsetY / ky) * ky;
        painter.translate(offsetX, offsetY);
        _gridRects.paint(&painter);
        painter.translate(-offsetX, -offsetY);
    }
    
    // draw pixels
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

RectList::RectList(int capacity, int r, int g, int b, int a) :
    _color(r, g, b, a),
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
        painter->fillRect(_rects[i], _color);
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

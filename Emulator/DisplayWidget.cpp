#include "DisplayWidget.h"

#include "drawing.h"
#include "game.h"
#include "Emulator.h"
#include "EditorTools.h"
#include "Constants.h"
#include "Level.h"

#include <QPainter>
#include <QDebug>

#define RECT_COLOR_PIXEL 0, 0, 0
#define RECT_COLOR_GRID  241, 245, 248

static const int GRID_ROWS = DISPLAY_WIDTH / GRID_CELL_WIDTH + 2;
static const int GRID_COLS = DISPLAY_HEIGHT / GRID_CELL_HEIGHT + 2;

DisplayWidget::DisplayWidget(QWidget *parent) :
    QOpenGLWidget(parent),
    _pixelRects(DISPLAY_WIDTH * DISPLAY_HEIGHT, RECT_COLOR_PIXEL),
    _gridRects(GRID_ROWS * GRID_COLS, RECT_COLOR_GRID),
    _currentTool(NULL),
    _gridVisible(false)
{
    setAutoFillBackground(false);
    setMouseTracking(true);
    
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
    RELEASE(_currentTool);
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

void DisplayWidget::drawCharaters(QPainter *painter)
{
    Level *level = Level::current();
    painter->setOpacity(0.5);

    // draw player
    drawCharater(painter, level->player());

    // draw enemies
    const QList<LevelCharacter> enemies = level->enemies();
    for (int i = 0; i < enemies.size(); ++i)
    {
        drawCharater(painter, enemies.at(i));
    }

    painter->setOpacity(1.0);
}

void DisplayWidget::drawCharater(QPainter *painter, const LevelCharacter &character)
{
    const QImage &image = character.image();
    int drawX = (character.x() + drawTransX) * PIXEL_WIDTH;
    int drawY = (character.y() + drawTransY) * PIXEL_HEIGHT;
    
    painter->drawImage(drawX, drawY, image);
}

void DisplayWidget::setTool(EditorTool *tool)
{
    EditorTool *oldTool = _currentTool;
    _currentTool = RETAIN(tool);
    if (_currentTool) _currentTool->start();
    if (oldTool) oldTool->stop();
    RELEASE(oldTool);
}

void DisplayWidget::setDefaultTool()
{
    EditorDrawTool *tool = new EditorDrawTool(this);
    setTool(tool);
    tool->release();
}

void DisplayWidget::paintEvent(QPaintEvent *)
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

    // draw enemies
    if (emulator.editMode())
    {
        drawCharaters(&painter);
    }
    
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

void DisplayWidget::leaveEvent(QEvent *)
{
    if (_currentTool)
    {
        _currentTool->mouseExited();
    }
}

void DisplayWidget::enterEvent(QEvent *)
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

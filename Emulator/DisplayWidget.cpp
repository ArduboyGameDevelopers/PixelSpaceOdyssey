#include "DisplayWidget.h"

#include "drawing.h"
#include "game.h"
#include "Emulator.h"
#include "EditorTools.h"
#include "Constants.h"
#include "Level.h"
#include "Params.h"

#include <Arduino.h>

#include <QPainter>
#include <QDebug>

#define RECT_COLOR_PIXEL 0, 0, 0
#define RECT_COLOR_GRID  241, 245, 248

static const int GRID_ROWS = DISPLAY_WIDTH / GRID_CELL_WIDTH + 2;
static const int GRID_COLS = DISPLAY_HEIGHT / GRID_CELL_HEIGHT + 2;

bool PARAM_COLLIDERS = false;
bool PARAM_SHOW_AI_VISION = false;
bool PARAM_SHOW_WALK_ZONES = false;
bool PARAM_SHOW_AI_TARGET = false;

static void drawBoundingBox(QPainter &painter, const Character &character);
static void drawTarget(QPainter &painter, const Character &character);

DisplayWidget::DisplayWidget(QWidget *parent) :
    QWidget(parent),
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
    const QList<CharacterInfo> enemies = level->enemies();
    for (int i = 0; i < enemies.size(); ++i)
    {
        drawCharater(painter, enemies.at(i));
    }

    painter->setOpacity(1.0);
}

void DisplayWidget::drawCharater(QPainter *painter, const CharacterInfo &character)
{
    const QImage &image = character.image();
    int drawX = character.x() * PIXEL_WIDTH - image.width() / 2;
    int drawY = character.y() * PIXEL_HEIGHT - image.height() / 2;
    
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

void DisplayWidget::focusCharacter(int index) const
{
    Level *level = Level::current();
    
    if (emulator.editMode())
    {
        if (index == 0)
        {
            camX = S2W(level->player().x());
            camY = S2W(level->player().y());
        }
        else if (index > 0 && index <= level->enemiesCount())
        {
            const CharacterInfo &enemy = level->enemies().at(index - 1);
            camX = S2W(enemy.x());
            camY = S2W(enemy.y());
        }
        
        int16_t maxCamX = tileMapWidth - CAM_WIDTH_HALF;
        int16_t maxCamY = tileMapHeight - CAM_HEIGHT_HALF;
        camX = constrain(camX, CAM_WIDTH_HALF, maxCamX);
        camY = constrain(camY, CAM_HEIGHT_HALF, maxCamY);
    }
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

    // debug info
    drawDebug(painter);
    
    // draw tool
    if (_currentTool)
    {
        _currentTool->paint(&painter);
    }

    painter.end();
}

void DisplayWidget::drawDebug(QPainter &painter)
{
    int drawOffsetX = drawTransX * PIXEL_WIDTH;
    int drawOffsetY = drawTransY * PIXEL_HEIGHT;
    painter.translate(drawOffsetX, drawOffsetY);
    
    // draw enemies
    if (emulator.editMode())
    {
        drawCharaters(&painter);
    }
    
    // draw bounding boxes
    if (PARAM_COLLIDERS)
    {
        for (int i = 0; i < enemiesCount; ++i)
        {
            drawBoundingBox(painter, enemies[i]);
        }
        
        drawBoundingBox(painter, player);
    }
    
    // draw sight areas
    if (PARAM_SHOW_AI_VISION)
    {
        QRgb color = qRgb(247, 148, 29);
        painter.setOpacity(0.25);
        for (int i = 0; i < enemiesCount; ++i)
        {
            const Character &enemy = enemies[i];
            int16_t minX = W2S(EnemyGetMinSightX(&enemy));
            int16_t maxX = W2S(EnemyGetMaxSightX(&enemy));
            
            int cx = minX * PIXEL_WIDTH;
            int cy = (W2S(enemy.y) - DIV2(TILE_HEIGHT_PX)) * PIXEL_HEIGHT;
            int sw = (maxX - minX) * PIXEL_WIDTH;
            int sh = TILE_HEIGHT_PX * PIXEL_HEIGHT;
            
            painter.fillRect(cx, cy, sw, sh, color);
        }
        painter.setOpacity(1.0);
    }
    
    // draw sight areas
    if (PARAM_SHOW_WALK_ZONES)
    {
        QRgb color = qRgb(0, 166, 81);
        painter.setOpacity(0.25);
        for (int i = 0; i < enemiesCount; ++i)
        {
            const Character &enemy = enemies[i];
            int cx = W2S(enemy.moveMinX - DIV2(enemy.width)) * PIXEL_WIDTH;
            int cy = W2S(CharacterGetTop(&enemy)) * PIXEL_HEIGHT;
            int sw = W2S(enemy.moveMaxX - enemy.moveMinX + enemy.width) * PIXEL_WIDTH;
            int sh = W2S(enemy.height) * PIXEL_HEIGHT;
            
            painter.fillRect(cx, cy, sw, sh, color);
        }
        painter.setOpacity(1.0);
    }
    
    // draw last seen player pos
    if (PARAM_SHOW_AI_TARGET)
    {
        QRgb visible = qRgb(237, 28, 36);
        QRgb notvisible = qRgb(246, 142, 86);
        painter.setOpacity(0.25);
        for (int i = 0; i < enemiesCount; ++i)
        {
            const Character &enemy = enemies[i];
            int cx = W2S(enemy.lastPlayerX - DIV2(player.width)) * PIXEL_WIDTH;
            int cy = W2S(enemy.y - DIV2(player.height)) * PIXEL_HEIGHT;
            int sw = TILE_WIDTH_PX * PIXEL_WIDTH;
            int sh = TILE_HEIGHT_PX * PIXEL_HEIGHT;
            
            painter.fillRect(cx, cy, sw, sh, enemy.canSeePlayer ? visible : notvisible);
        }
        painter.setOpacity(1.0);

        for (int i = 0; i < enemiesCount; ++i)
        {
            drawTarget(painter, enemies[i]);
        }
    }
    
    painter.translate(-drawOffsetX, -drawOffsetY);
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

static void drawBoundingBox(QPainter &painter, const Character &character)
{
    int x = W2S(character.x) * PIXEL_WIDTH;
    int y = W2S(character.y) * PIXEL_HEIGHT;
    
    int w = W2S(character.width) * PIXEL_WIDTH;
    int h = W2S(character.height) * PIXEL_HEIGHT;
    int cx = x - DIV2(w);
    int cy = y - DIV2(h);
    painter.setPen(Qt::red);
    painter.drawLine(x - 1, y, x + 1, y);
    painter.drawLine(x, y - 1, x, y + 1);
    painter.drawRect(QRect(cx, cy, w, h));
}

static void drawTarget(QPainter &painter, const Character &character)
{
    int width = W2S(character.width) * PIXEL_WIDTH;
    int height = W2S(character.height) * PIXEL_WIDTH;
    
    int x = W2S(character.targetPos) * PIXEL_WIDTH + character.dir * DIV2(width);
    int y = W2S(character.y) * PIXEL_HEIGHT;

    painter.setPen(Qt::blue);
    painter.drawLine(x, y - DIV2(height), x, y + DIV2(height));
    painter.fillRect(x, y - DIV2(height), 18, 9, Qt::red);
}

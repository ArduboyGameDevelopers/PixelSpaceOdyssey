#ifndef DISPLAYWIDGET_H
#define DISPLAYWIDGET_H

#include <QtGlobal>
#include <QOpenGLWidget>
#include <QPainter>

extern const int GRID_CELL_SIZE;
extern const int PIXEL_WIDTH;
extern const int PIXEL_HEIGHT;

typedef QRect Rect;

class RectList
{
public:
    RectList(int capacity, int r, int g, int b, int a = 255);
    virtual ~RectList();

public:
    void paint(QPainter* painter);
    void clear();
    void add(int x, int y, int w, int h);

public:
    inline int capacity() const { return _capacity; }
    inline int count() const { return _count; }

private:
    Rect*  _rects;
    QColor _color;
    int    _capacity;
    int    _count;
};

class EditorTool;

class DisplayWidget : public QOpenGLWidget
{
    Q_OBJECT
public:
    explicit DisplayWidget(QWidget *parent = 0);
    virtual ~DisplayWidget();

public:
    void copyScreenBuffer(unsigned const char* screenBuffer, int bufferWidth, int bufferHeight);

public:
    void startPan();
    void stopPan();
    
    EditorTool* currentTool() const { return _currentTool; }
    void setCurrentTool(EditorTool* tool) { _currentTool = tool; }

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
    void keyReleaseEvent(QKeyEvent *event) Q_DECL_OVERRIDE;

    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

    void leaveEvent(QEvent *event) Q_DECL_OVERRIDE;
    void enterEvent(QEvent *event)  Q_DECL_OVERRIDE;

public:
    inline bool gridVisible() const { return _gridVisible; }
    inline void setGridVisible(bool gridVisible) { _gridVisible = gridVisible; }

private:
    RectList _pixelRects;
    RectList _gridRects;
    EditorTool* _currentTool;
    bool _gridVisible;
};

#endif // DISPLAYWIDGET_H

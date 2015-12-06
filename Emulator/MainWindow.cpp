#include "MainWindow.h"
#include "ui_mainwindow.h"

#include <QDateTime>
#include <QTimer>
#include <QKeyEvent>
#include <QDebug>

#include "bridge.h"
#include "platform.h"
#include "Level.h"
#include "game.h"

#include "GameInput.h"

#include "EditorTools.h"

static const int kTimerDelay = 1000 / 60;

Emulator emulator;
static MainWindow* _instance = NULL;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    _ui(new Ui::MainWindow),
    _displayWidget(NULL),
    _lastTool(NULL),
    _lastFrameTime(0)
{
    _instance = this;

    _ui->setupUi(this);
    _displayWidget = _ui->displayWidget;
    
    setupToolBar(_ui->toolBar);
    setupActions();
    setupTileSet(_ui->tilesWidget);

    Level *level = new Level(tileMap.indices, tileMap.rows, tileMap.cols);
    Level::setCurrent(level);
    level->release();

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(runUpdate()));
    timer->start(kTimerDelay);

    emulator.start();
    _lastFrameTime = QDateTime::currentMSecsSinceEpoch();
}

MainWindow::~MainWindow()
{
    _instance = NULL;
    RELEASE(_lastTool);
    delete _ui;
}

void MainWindow::runUpdate()
{
    qint64 currentTime = QDateTime::currentMSecsSinceEpoch();
    qint64 dt = currentTime - _lastFrameTime;
    _lastFrameTime = currentTime;
    emulator.update(dt);
}

void MainWindow::copyScreenBuffer(const unsigned char *screenBuffer, int bufferWidth, int bufferHeight)
{
    _displayWidget->copyScreenBuffer(screenBuffer, bufferWidth, bufferHeight);
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    int key = event->key();
    emulator.buttonPressed(getButtonFromKey(key));
 
    if (event->isAutoRepeat()) return;
    
    if (emulator.editMode())
    {
        switch (key) {
            case Qt::Key_Space:
                EditorPanTool *panTool = new EditorPanTool(displayWidget());
                pushEditorTool(panTool);
                panTool->release();
                break;
        }
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    int key = event->key();
    emulator.buttonReleased(getButtonFromKey(key));
    
    if (event->isAutoRepeat()) return;
    
    if (emulator.editMode())
    {
        switch (key) {
            case Qt::Key_Space:
                popEditorTool();
                break;
        }
    }
}

void MainWindow::setEditorTool(EditorTool *tool)
{
    _displayWidget->setTool(tool);
}

void MainWindow::pushEditorTool(EditorTool *tool)
{
    Q_ASSERT(_lastTool == NULL);
    _lastTool = RETAIN(displayWidget()->currentTool());
    
    setEditorTool(tool);
}

void MainWindow::popEditorTool()
{
    setEditorTool(_lastTool);
    RELEASE(_lastTool);
}

void platformRenderScreen(unsigned const char* screenBuffer, int width, int height)
{
    _instance->copyScreenBuffer(screenBuffer, width, height);
}

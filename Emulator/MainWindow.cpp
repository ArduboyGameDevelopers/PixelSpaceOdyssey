#include "MainWindow.h"
#include "ui_mainwindow.h"

#include <QDateTime>
#include <QTimer>
#include <QKeyEvent>
#include <QDebug>

#include "bridge.h"
#include "platform.h"
#include "Level.h"
#include "EditorState.h"
#include "game.h"

#include "GameInput.h"

static const int kTimerDelay = 1000 / 60;

Emulator emulator;
static MainWindow* _instance = NULL;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    _ui(new Ui::MainWindow),
    _displayWidget(NULL),
    _lastFrameTime(0)
{
    _instance = this;

    _ui->setupUi(this);
    _displayWidget = _ui->displayWidget;
    
    setupToolBar(_ui->toolBar);
    setupActions();

    editorState.level = new Level(tileMap.indices, tileMap.rows, tileMap.cols);

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(runUpdate()));
    timer->start(kTimerDelay);

    emulator.start();
    _lastFrameTime = QDateTime::currentMSecsSinceEpoch();
}

MainWindow::~MainWindow()
{
    _instance = NULL;
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
                _displayWidget->startPan();
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
                _displayWidget->stopPan();
                break;
        }
    }
}

void platformRenderScreen(unsigned const char* screenBuffer, int width, int height)
{
    _instance->copyScreenBuffer(screenBuffer, width, height);
}

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
#include "Settings.h"

#include "EditorTools.h"
#include "CharacterButton.h"
#include "CharacterListView.h"

#include "Version.h"

static const int kTimerDelay = 1000 / 60;

MainWindow *MainWindow::_instance(NULL);

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    _ui(new Ui::MainWindow),
    _displayWidget(NULL),
    _lastTool(NULL),
    _lastFrameTime(0),
    _characterButtons()
{
    _instance = this;
    setFocusPolicy(Qt::StrongFocus);
    
    _ui->setupUi(this);
    _displayWidget = _ui->displayWidget;
    
    loadTiles();
    
    setupToolBar(_ui->toolBar);
    setupActions();
    setupCharacterList();
    
    emulator.start();
    
    Level *level = NULL;
    const QString &lastLevel = Settings::getString(kSettingsLastLevel, "");
    if (lastLevel.length() > 0)
    {
        level = Level::readFromFile(lastLevel);
    }

    if (level == NULL)
    {
        level = new Level(tileMap.indices, tileMap.rows, tileMap.cols);
        level->setPlayerPos(W2S(player.x), W2S(player.y));
        level->setPlayerDir((CharacterDir) player.dir);
    }
    Level::setCurrent(level);
    level->release();

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(runUpdate()));
    timer->start(kTimerDelay);

    _lastFrameTime = QDateTime::currentMSecsSinceEpoch();
    
    setFocus();
}

MainWindow::~MainWindow()
{
    _instance = NULL;
    RELEASE(_lastTool);
    delete _ui;
}

void MainWindow::loadTiles()
{
    QDir tilesDir("Tiles");
    if (!tilesDir.exists())
    {
        QMessageBox::critical(NULL, "Error", "Can't find 'Tiles' directory");
    }
    
    QComboBox *tilesComboBox = _ui->tilesComboBox;
    
    QFileInfoList files = tilesDir.entryInfoList(QStringList("*.png"));
    for (int i = 0; i < files.size(); ++i)
    {
        TileSet* tileSet = new TileSet(files.at(i).filePath());
        editorState.addTileSet(tileSet);
        tilesComboBox->addItem(tileSet->name());
        tileSet->release();
    }
    
    selectTileSet(0);
    
    connect(tilesComboBox, SIGNAL(activated(int)), this, SLOT(onTileSetSelected(int)));
}

void MainWindow::selectTileSet(int index)
{
    editorState.setTileSetIndex(index);
    
    TileSet *scaledTileSet = new TileSet(*editorState.currentTileSet(), 4);
    _ui->tilesWidget->setTileSet(scaledTileSet);
    scaledTileSet->release();
}

void MainWindow::onTileSetSelected(int index)
{
    selectTileSet(index);
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

void MainWindow::updateLevelUi(Level *level)
{
    CharacterListView *characterListView = _ui->characterListView;
    characterListView->updateItem(level);
    
    updateWindowTitle(level);
}

void MainWindow::updateWindowTitle(Level *level)
{
    const QString &basename = QFileInfo(level->filename()).baseName();
    const QString &name = basename.length() > 0 ? basename : "Untitled";
    
    setWindowTitle(QString("Pixel Space Oddysey %1 - %2").arg(PROJECT_VERSION, name));
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
    RELEASE(_lastTool); _lastTool = NULL;
}

void platformRenderScreen(unsigned const char* screenBuffer, int width, int height)
{
    MainWindow::instance()->copyScreenBuffer(screenBuffer, width, height);
}

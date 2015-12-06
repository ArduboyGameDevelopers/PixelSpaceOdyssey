#include "MainWindow.h"
#include "DisplayWidget.h"
#include "ui_mainwindow.h"
#include "Tileset.h"
#include "EditorTools.h"
#include "EditorState.h"
#include "Settings.h"

#include "Arduboy.h"

#include <QFileDialog>

void MainWindow::setupActions()
{
    QAction *actionNew    = _ui->actionNew;
    QAction *actionOpen   = _ui->actionOpen;
    QAction *actionSave   = _ui->actionSave;
    QAction *actionSaveAs = _ui->actionSaveAs;
    QAction *actionPlay   = _ui->actionPlay;
    QAction *actionPause  = _ui->actionPause;
    QAction *actionStep   = _ui->actionStep;
    QAction *actionGrid   = _ui->actionGrid;
    QAction *actionEdit   = _ui->actionEdit;

    // set state
    setPauseMode(emulator.paused());

    // connect slots
    connect(actionNew,    SIGNAL(triggered()),     this, SLOT(onActionNew()));
    connect(actionOpen,   SIGNAL(triggered()),     this, SLOT(onActionOpen()));
    connect(actionSave,   SIGNAL(triggered()),     this, SLOT(onActionSave()));
    connect(actionSaveAs, SIGNAL(triggered()),     this, SLOT(onActionSaveAs()));
    connect(actionStep,   SIGNAL(triggered()),     this, SLOT(onActionStep()));
    connect(actionPlay,   SIGNAL(triggered()),     this, SLOT(onActionPlay()));
    connect(actionPause,  SIGNAL(triggered()),     this, SLOT(onActionPause()));
    connect(actionEdit,   SIGNAL(triggered(bool)), this, SLOT(onActionEdit(bool)));
    connect(actionGrid,   SIGNAL(toggled(bool)),   this, SLOT(onActionToggleGrid(bool)));
    
    // grid
    bool gridVisible = Settings::getBool(kSettingsGridVisible);
    displayWidget()->setGridVisible(gridVisible);
    actionGrid->setChecked(gridVisible);
}

void MainWindow::setupTileSet(TilesWidget *tilesWidget)
{
    QImageReader imageReader("/Users/weee/dev/projects/arduboy/games/PixelSpaceOdysspy/Emulator/Images/tiles.png");
    QImage tilesImage = imageReader.read();
    TileSet* tileSet = new TileSet(tilesImage);
    tilesWidget->setTileSet(tileSet);
    tileSet->release();
}

void MainWindow::setPauseMode(bool pauseMode)
{
    emulator.setPaused(pauseMode);
    if (!pauseMode)
    {
        setEditMode(false);
    }
    
    _ui->actionPause->setChecked(pauseMode);
    _ui->actionPlay->setChecked(!pauseMode);
}

void MainWindow::setEditMode(bool editMode)
{
    emulator.setEditMode(editMode);
    if (editMode)
    {
        setPauseMode(true);
    }
    
    _ui->actionEdit->setChecked(editMode);
    
    EditorTool *tool = editMode ? new EditorDrawTool(displayWidget()) : NULL;
    setEditorTool(tool);
    RELEASE(tool);
}

void MainWindow::onActionNew()
{
    int rows = HEIGHT / GRID_CELL_HEIGHT;;
    int cols = WIDTH / GRID_CELL_WIDTH;
    int indexCount = rows * cols;
    
    uint8_t indices[indexCount];
    memset(indices, 0, sizeof(indices));
    
    for (int i = 0, j1 = 0, j2 = cols - 1; i < rows; ++i)
    {
        indices[i * cols + j1] = 16;
        indices[i * cols + j2] = 16;
    }
    
    for (int i1 = 0, i2 = rows - 1, j = 0; j < cols; ++j)
    {
        indices[i1 * cols + j] = 16;
        indices[i2 * cols + j] = 16;
    }
    
    player.x = S2W(cols / 2 * GRID_CELL_WIDTH);
    player.y = S2W(rows / 2 * GRID_CELL_HEIGHT);
    
    Level *level = new Level(indices, rows, cols);
    tileMap.indices = level->indices();
    tileMap.rows = level->rows();
    tileMap.cols = level->cols();
    
    tileMapWidth = S2W(cols * GRID_CELL_WIDTH);
    tileMapHeight = S2W(rows * GRID_CELL_HEIGHT);
}

void MainWindow::onActionOpen()
{
    QString filename = QFileDialog::getOpenFileName(NULL, "Open Level", "", "Level Files (*.pso)");
    if (filename.length() > 0)
    {
        Level *level = Level::readFromFile(filename);
        editorState.level = level;
        tileMap.indices = level->indices();
        tileMap.rows = level->rows();
        tileMap.cols = level->cols();
    }
}

void MainWindow::onActionSave()
{
    editorState.level->writeToFile("level.pso");
}

void MainWindow::onActionSaveAs()
{
    QString filename = QFileDialog::getSaveFileName(NULL, "Save Level As", "", "Level Files (*.pso)");
}

void MainWindow::onActionPlay()
{
    setPauseMode(false);
}

void MainWindow::onActionPause()
{
    setPauseMode(true);
}

void MainWindow::onActionEdit(bool selected)
{
    if (selected)
    {
        setEditMode(true);
    }
    else
    {
        setPauseMode(false);
    }
}

void MainWindow::onActionStep()
{
    setEditMode(false);
    setPauseMode(true);
    
    emulator.step();
}

void MainWindow::onActionToggleGrid(bool selected)
{
    displayWidget()->setGridVisible(selected);
    Settings::setBool(kSettingsGridVisible, selected);
}

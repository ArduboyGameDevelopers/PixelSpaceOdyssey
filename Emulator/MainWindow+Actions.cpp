#include "MainWindow.h"
#include "DisplayWidget.h"
#include "ui_mainwindow.h"
#include "Tileset.h"
#include "EditorTools.h"
#include "Settings.h"
#include "Constants.h"
#include "EditorState.h"
#include "Level.h"
#include "CharacterListView.h"

#include "Input2Dialog.h"
#include <QFileDialog>

void MainWindow::setupActions()
{
    QAction *actionNew    = _ui->actionNew;
    QAction *actionOpen   = _ui->actionOpen;
    QAction *actionSave   = _ui->actionSave;
    QAction *actionSaveAs = _ui->actionSaveAs;
    QAction *actionSize   = _ui->actionSize;
    QAction *actionOffset = _ui->actionOffset;
    QAction *actionImport = _ui->actionImportImage;
    QAction *actionExport = _ui->actionExportImage;
    QAction *actionPlay   = _ui->actionPlay;
    QAction *actionPause  = _ui->actionPause;
    QAction *actionStep   = _ui->actionStep;
    QAction *actionGrid   = _ui->actionGrid;
    QAction *actionEdit   = _ui->actionEdit;
    QAction *actionImportTileset = _ui->actionImportTileset;

    // set state
    setPauseMode(emulator.paused());

    // connect slots
    connect(actionNew,           SIGNAL(triggered()),     this, SLOT(onActionNew()));
    connect(actionOpen,          SIGNAL(triggered()),     this, SLOT(onActionOpen()));
    connect(actionSave,          SIGNAL(triggered()),     this, SLOT(onActionSave()));
    connect(actionSaveAs,        SIGNAL(triggered()),     this, SLOT(onActionSaveAs()));
    connect(actionSize,          SIGNAL(triggered()),     this, SLOT(onActionSize()));
    connect(actionOffset,        SIGNAL(triggered()),     this, SLOT(onActionOffset()));
    connect(actionImportTileset, SIGNAL(triggered()),     this, SLOT(onActionImportTileSet()));
    connect(actionImport,        SIGNAL(triggered()),     this, SLOT(onActionImport()));
    connect(actionExport,        SIGNAL(triggered()),     this, SLOT(onActionExport()));
    connect(actionStep,          SIGNAL(triggered()),     this, SLOT(onActionStep()));
    connect(actionPlay,          SIGNAL(triggered()),     this, SLOT(onActionPlay()));
    connect(actionPause,         SIGNAL(triggered()),     this, SLOT(onActionPause()));
    connect(actionEdit,          SIGNAL(triggered(bool)), this, SLOT(onActionEdit(bool)));
    connect(actionGrid,          SIGNAL(toggled(bool)),   this, SLOT(onActionToggleGrid(bool)));

    // character buttons
    _ui->characterButtonPlayer->setCharacterType(CharacterTypePlayer);
    _characterButtons.append(_ui->characterButtonPlayer);
    
    _ui->characterButtonBear->setCharacterType(CharacterTypeBear);
    _characterButtons.append(_ui->characterButtonBear);
    
    _ui->characterButtonDog->setCharacterType(CharacterTypeDog);
    _characterButtons.append(_ui->characterButtonDog);
    
    _ui->characterButtonSpiderSmall->setCharacterType(CharacterTypeSpiderSmall);
    _characterButtons.append(_ui->characterButtonSpiderSmall);
    
    _ui->characterButtonSpiderBig->setCharacterType(CharacterTypeSpiderLarge);
    _characterButtons.append(_ui->characterButtonSpiderBig);
    
    for (int i = 0; i < _characterButtons.size(); ++i)
    {
        connect(_characterButtons[i], SIGNAL(clicked(bool)), this, SLOT(onCharacterButton(bool)));
    }
    
    // grid
    bool gridVisible = Settings::getBool(kSettingsGridVisible);
    displayWidget()->setGridVisible(gridVisible);
    actionGrid->setChecked(gridVisible);
}

void MainWindow::setupCharacterList()
{
    CharacterListView *characterList = _ui->characterListView;
    connect(characterList, SIGNAL(clicked(const QModelIndex &)), this, SLOT(onCharacterListItemClicked(QModelIndex)));
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
    int rows = DISPLAY_HEIGHT / GRID_CELL_HEIGHT;;
    int cols = DISPLAY_WIDTH / GRID_CELL_WIDTH;
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
    
    Level *level = new Level(indices, rows, cols);
    level->setPlayerPos(cols / 2 * GRID_CELL_WIDTH, cols / 2 * GRID_CELL_WIDTH);
    level->setPlayerDir(CharacterDirRight);
    Level::setCurrent(level);
    level->release();
}

void MainWindow::onActionOpen()
{
    QString filename = QFileDialog::getOpenFileName(NULL, "Open Level", "", "Level Files (*.pso)");
    if (filename.length() > 0)
    {
        Level *level = Level::readFromFile(filename);
        Level::setCurrent(level);
        level->release();
    }
}

void MainWindow::onActionSave()
{
    Level::current()->writeToFile("level.pso");
}

void MainWindow::onActionSaveAs()
{
    QString filename = QFileDialog::getSaveFileName(NULL, "Save Level As", "", "Level Files (*.pso)");
}

void MainWindow::onActionSize()
{
    Input2Dialog dialog(this, "Width:", "Height:");
    dialog.setFirst(QString::number(Level::current()->cols()));
    dialog.setSecond(QString::number(Level::current()->rows()));
    int result = dialog.exec();
    if (result == QDialog::Accepted)
    {
        int cols = dialog.first().toInt();
        int rows = dialog.second().toInt();
        Level::current()->resize(rows, cols);
    }
}

void MainWindow::onActionOffset()
{
    
}

void MainWindow::onActionImportTileSet()
{
    QString filename = QFileDialog::getOpenFileName(NULL, "Import Tileset", "", "Image files (*.png)");
    if (filename.length() > 0)
    {
        QImage image(filename);

        TileSet *tileSet = new TileSet("foo", image);
        editorState.addTileSet(tileSet);
        tileSet->release();
    }
}

void MainWindow::onActionImport()
{
    QString filename = QFileDialog::getOpenFileName(NULL, "Import Level", "", "Image files (*.png)");
    if (filename.length() > 0)
    {
        QImage image(filename);
        
        Level *level = Level::readFromImage(image, editorState.currentTileSet());
        Level::setCurrent(level);
        level->release();
    }
}

void MainWindow::onActionExport()
{
    QString filename = QFileDialog::getSaveFileName(NULL, "Save Level", "", "Image files (*.png)");
    if (filename.length() == 0)
    {
        return;
    }
    
    Level *level = Level::current();
    TileSet *tileSet = editorState.currentTileSet();
    
    const uint8_t *indices = level->indices();
    int rows = level->rows();
    int cols = level->cols();
    
    QImage image(cols * GRID_CELL_WIDTH, rows * GRID_CELL_HEIGHT, QImage::Format_RGB32);
    
    int index = 0;
    QPainter painter(&image);
    painter.fillRect(0, 0, image.width(), image.height(), Qt::white);
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            int tileIndex = indices[index++];
            if (tileIndex > 0)
            {
                tileSet->drawTile(&painter, tileIndex - 1, j * tileSet->tileWidth(), i * tileSet->tileHeight());
            }
        }
    }
    
    image.save(filename, "PNG");
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

void MainWindow::onCharacterButton(bool checked)
{
    if (checked)
    {
        setEditMode(true);
        
        CharacterButton *button = dynamic_cast<CharacterButton *>(QObject::sender());
        button->deselectOther();
        button->setChecked(true);

        EditorCharTool *tool = new EditorCharTool(displayWidget(), button->characterType());
        setEditorTool(tool);
        tool->release();
    }
}

void MainWindow::onCharacterListItemClicked(const QModelIndex & index)
{
    int characterIndex = index.row();
    
    editorState.setCharacterIndex(characterIndex);
    displayWidget()->focusCharacter(characterIndex);
}

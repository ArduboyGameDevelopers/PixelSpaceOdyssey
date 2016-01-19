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
#include "Input2Dialog.h"
#include "ResizeDialog.h"

#include "Version.h"
#include "Params.h"

#include "debug.h"

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
    setupParamUI();
    
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
        level->setPlayerDir((Direction) player.dir);
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

#pragma mark -
#pragma mark Update

void MainWindow::runUpdate()
{
    qint64 currentTime = QDateTime::currentMSecsSinceEpoch();
    qint64 dt = currentTime - _lastFrameTime;
    _lastFrameTime = currentTime;
    emulator.update(dt);
}

#pragma mark -
#pragma mark Events

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

#pragma mark -
#pragma mark Screen Buffer

void MainWindow::copyScreenBuffer(const unsigned char *screenBuffer, int bufferWidth, int bufferHeight)
{
    _displayWidget->copyScreenBuffer(screenBuffer, bufferWidth, bufferHeight);
}

#pragma mark -
#pragma mark Platform

void platformRenderScreen(unsigned const char* screenBuffer, int width, int height)
{
    MainWindow::instance()->copyScreenBuffer(screenBuffer, width, height);
}

#pragma mark -
#pragma mark Emulator

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
    else
    {
        for (int i = 0; i < enemiesCount; ++i)
        {
            UpdateConstraints(&enemies[i]);
        }
    }
    
    _ui->actionEdit->setChecked(editMode);
    
    EditorTool *tool = editMode ? new EditorDrawTool(displayWidget()) : NULL;
    setEditorTool(tool);
    RELEASE(tool);
}

#pragma mark -
#pragma mark Tiles

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

#pragma mark -
#pragma mark Level UI

void MainWindow::updateLevelUi(Level *level)
{
    CharacterListView *characterListView = _ui->characterListView;
    characterListView->updateItem(level);
    
    updateWindowTitle(level);
    
    _ui->playerHealthEdit->setText(QString::number(playerHealth));
}

void MainWindow::updateWindowTitle(Level *level)
{
    const QString &basename = QFileInfo(level->filename()).baseName();
    const QString &name = basename.length() > 0 ? basename : "Untitled";
    
    setWindowTitle(QString("Pixel Space Oddysey %1 - %2").arg(PROJECT_VERSION, name));
}

#pragma mark -
#pragma mark Editor Tools

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

#pragma mark -
#pragma mark Toolbar

void MainWindow::setupToolBar(QToolBar *toolBar)
{
    QWidget* emptyLeft = new QWidget();
    emptyLeft->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    toolBar->insertWidget(_ui->actionPlay, emptyLeft);
    
    QWidget* emptyRight = new QWidget();
    emptyRight->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    toolBar->insertWidget(_ui->actionEdit, emptyRight);
}

#pragma mark -
#pragma mark Actions

void MainWindow::setupActions()
{
    QAction *actionNew    = _ui->actionNew;
    QAction *actionOpen   = _ui->actionOpen;
    QAction *actionSave   = _ui->actionSave;
    QAction *actionSaveAs = _ui->actionSaveAs;
    QAction *actionRestart= _ui->actionRestart;
    QAction *actionResize = _ui->actionResize;
    QAction *actionOffset = _ui->actionOffset;
    QAction *actionImport = _ui->actionImportImage;
    QAction *actionExport = _ui->actionExportImage;
    QAction *actionPlay   = _ui->actionPlay;
    QAction *actionPause  = _ui->actionPause;
    QAction *actionStep   = _ui->actionStep;
    QAction *actionGrid   = _ui->actionGrid;
    QAction *actionEdit   = _ui->actionEdit;
    QAction *actionImportTileset = _ui->actionImportTileset;
    QAction *actionSnapshot = _ui->actionSnapshot;
    
    // set state
    setPauseMode(emulator.paused());
    
    // connect slots
    connect(actionNew,           SIGNAL(triggered()),     this, SLOT(onActionNew()));
    connect(actionOpen,          SIGNAL(triggered()),     this, SLOT(onActionOpen()));
    connect(actionSave,          SIGNAL(triggered()),     this, SLOT(onActionSave()));
    connect(actionSaveAs,        SIGNAL(triggered()),     this, SLOT(onActionSaveAs()));
    connect(actionRestart,       SIGNAL(triggered()),     this, SLOT(onActionRestart()));
    connect(actionResize,        SIGNAL(triggered()),     this, SLOT(onActionResize()));
    connect(actionOffset,        SIGNAL(triggered()),     this, SLOT(onActionOffset()));
    connect(actionImportTileset, SIGNAL(triggered()),     this, SLOT(onActionImportTileSet()));
    connect(actionImport,        SIGNAL(triggered()),     this, SLOT(onActionImport()));
    connect(actionExport,        SIGNAL(triggered()),     this, SLOT(onActionExport()));
    connect(actionSnapshot,      SIGNAL(triggered()),     this, SLOT(onActionSnapshot()));
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
    
    // delete enemy buttons
    connect(_ui->enemyDeleteButton, SIGNAL(clicked()), this, SLOT(onDeleteEnemyButton()));
    
    // direction selection
    connect(_ui->leftRadioButton, SIGNAL(clicked(bool)), this, SLOT(onDirectionButtonChecked(bool)));
    connect(_ui->rightRadioButton, SIGNAL(clicked(bool)), this, SLOT(onDirectionButtonChecked(bool)));
    
    // grid
    bool gridVisible = Settings::getBool(kSettingsGridVisible);
    displayWidget()->setGridVisible(gridVisible);
    actionGrid->setChecked(gridVisible);
    
    // player health
    connect(_ui->playerHealthEdit, SIGNAL(returnPressed()), this, SLOT(playerHealthEditReturnPressed()));
}

void MainWindow::onActionNew()
{
    int rows = DISPLAY_HEIGHT / GRID_CELL_HEIGHT;;
    int cols = DISPLAY_WIDTH / GRID_CELL_WIDTH;
    int indexCount = rows * cols;
    
    const int kSolidTile = 31;
    
    uint8_t indices[indexCount];
    memset(indices, 0, sizeof(indices));
    
    for (int i = 0, j1 = 0, j2 = cols - 1; i < rows; ++i)
    {
        indices[i * cols + j1] = kSolidTile;
        indices[i * cols + j2] = kSolidTile;
    }
    
    for (int i1 = 0, i2 = rows - 1, j = 0; j < cols; ++j)
    {
        indices[i1 * cols + j] = kSolidTile;
        indices[i2 * cols + j] = kSolidTile;
    }
    
    Level *level = new Level(indices, rows, cols);
    level->setPlayerPos(cols / 2 * GRID_CELL_WIDTH, cols / 2 * GRID_CELL_WIDTH);
    level->setPlayerDir(DIR_RIGHT);
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
        
        Settings::setString(kSettingsLastLevel, filename);
    }
}

void MainWindow::onActionSave()
{
    Level *level = Level::current();
    if (level->hasFilename())
    {
        level->writeToFile(level->filename());
        Settings::setString(kSettingsLastLevel, level->filename());
    }
    else
    {
        onActionSaveAs();
    }
}

void MainWindow::onActionSaveAs()
{
    QString filename = QFileDialog::getSaveFileName(NULL, "Save Level As", "", "Level Files (*.pso)");
    if (filename.length() > 0)
    {
        Level *level = Level::current();
        level->writeToFile(filename);
        Settings::setString(kSettingsLastLevel, filename);
        updateWindowTitle(level);
    }
}

void MainWindow::onActionResize()
{
    ResizeDialog dialog(this);
    int result = dialog.exec();
    if (result == QDialog::Accepted)
    {
    }
}

void MainWindow::onActionOffset()
{
    
}

void MainWindow::onActionRestart()
{
    Level::restart();
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

void MainWindow::onActionSnapshot()
{
    DisplayWidget *widget = displayWidget();
    QPixmap pixmap(widget->size());
    widget->render(&pixmap);
    
    QDir dir;
    QFileInfoList list = dir.entryInfoList(QStringList("screenshot-*.png"));
    
    pixmap.save(QString("screenshot-%1.png").arg(list.size()));
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

void MainWindow::onDeleteEnemyButton()
{
    Level *level = Level::current();
    
    int enemyIndex = editorState.characterIndex() - 1; // player is at pos 0
    if (enemyIndex >= 0 && enemyIndex < level->enemiesCount())
    {
        level->deleteEnemy(enemyIndex);
    }
}

void MainWindow::onCharacterListItemClicked(const QModelIndex & index)
{
    int characterIndex = index.row();
    
    editorState.setCharacterIndex(characterIndex);
    displayWidget()->focusCharacter(characterIndex);
    updateDirectionalRadioButtons();
}

void MainWindow::playerHealthEditReturnPressed()
{
    bool succeed;
    int health = _ui->playerHealthEdit->text().toInt(&succeed);
    if (succeed)
    {
        ::playerHealth = health > kPlayerHealthMax ? kPlayerHealthMax : health;
        if (health == 0)
        {
            playerDie();
        }
        
        setFocus();
    }
}

#pragma mark -
#pragma mark Characters

void MainWindow::setupCharacterList()
{
    CharacterListView *characterList = _ui->characterListView;
    connect(characterList, SIGNAL(clicked(const QModelIndex &)), this, SLOT(onCharacterListItemClicked(QModelIndex)));
}

void MainWindow::onDirectionButtonChecked(bool checked)
{
    int characterIndex = editorState.characterIndex();
    if (characterIndex == -1)
    {
        return;
    }
    
    if (checked)
    {
        Direction dir;
        
        QRadioButton *radioButton = dynamic_cast<QRadioButton *>(QWidget::sender());
        if (radioButton == _ui->leftRadioButton)
        {
            dir = DIR_LEFT;
        }
        else /* if (radioButton == _ui->rightRadioButton) */
        {
            dir = DIR_RIGHT;
        }
        
        Level *level = Level::current();
        if (characterIndex == 0)
        {
            level->player().setDirection(dir);
        }
        else
        {
            int enemyIndex = characterIndex - 1;
            level->setEnemyDir(enemyIndex, dir);
        }
    }
}

void MainWindow::updateDirectionalRadioButtons()
{
    int characterIndex = editorState.characterIndex();
    if (characterIndex == -1)
    {
        _ui->rightRadioButton->setEnabled(false);
        _ui->leftRadioButton->setEnabled(false);
        
        _ui->rightRadioButton->setChecked(false);
        _ui->leftRadioButton->setChecked(false);
    }
    else
    {
        _ui->rightRadioButton->setEnabled(true);
        _ui->leftRadioButton->setEnabled(true);
        
        const Level *level = Level::current();
        Direction direction;
        if (characterIndex == 0)
        {
            direction = level->player().direction();
        }
        else
        {
            int enemyIndex = characterIndex - 1;
            direction = level->enemies().at(enemyIndex).direction();
        }
        
        _ui->rightRadioButton->setChecked(direction == DIR_RIGHT);
        _ui->leftRadioButton->setChecked(direction == DIR_LEFT);
    }
}

#pragma mark -
#pragma mark Param UI

void MainWindow::setupParamUI()
{
    _ui->showCollidersCheckBox->setParamPtr(&PARAM_COLLIDERS);
    _ui->showWalkZonesCheckBox->setParamPtr(&PARAM_SHOW_WALK_ZONES);
    _ui->showAIVisionCheckBox->setParamPtr(&PARAM_SHOW_AI_VISION);
    _ui->showAITargetCheckBox->setParamPtr(&PARAM_SHOW_AI_TARGET);
}

#pragma mark -
#pragma mark Debug

void dispatchDebugEvent(const char *name)
{
    if (strcmp(name, DEBUG_EVENT_PLAYER_DAMAGE) == 0)
    {
        MainWindow::instance()->ui()->playerHealthEdit->setText(QString::number(::playerHealth));
    }
}

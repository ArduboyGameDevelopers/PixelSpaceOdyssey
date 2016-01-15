#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGlobal>
#include <QtWidgets>

#include "Emulator.h"

namespace Ui {
class MainWindow;
}

class QToolBar;
class DisplayWidget;
class TilesWidget;
class EditorTool;
class CharacterButton;
class Level;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void runUpdate();

    void onActionNew();
    void onActionOpen();
    void onActionSave();
    void onActionSaveAs();
    
    void onActionResize();
    void onActionOffset();

    void onActionRestart();
    void onActionImportTileSet();
    void onActionImport();
    void onActionExport();
    void onActionSnapshot();

    void onActionPlay();
    void onActionPause();
    void onActionEdit(bool selected);
    void onActionStep();

    void onActionToggleGrid(bool selected);
    void onCharacterButton(bool);
    void onDeleteEnemyButton();
    
    void onTileSetSelected(int index);
    void onDirectionButtonChecked(bool checked);
    void onCharacterListItemClicked(const QModelIndex & index);
    
    void playerHealthEditReturnPressed();

public:
    void copyScreenBuffer(unsigned const char* screenBuffer, int bufferWidth, int bufferHeight);
    
public:
    inline static MainWindow* instance() { return _instance; }
    inline DisplayWidget *displayWidget() const { return _displayWidget; }
    inline Ui::MainWindow* ui() const { return _ui; }
    
public:
    void updateLevelUi(Level *level);
    void updateWindowTitle(Level *level);

protected:
    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
    void keyReleaseEvent(QKeyEvent *event) Q_DECL_OVERRIDE;

private:
    void loadTiles();
    void selectTileSet(int index);
    
    void setupToolBar(QToolBar* toolBar);
    void setupActions();
    void setupCharacterList();
    void setupParamUI();
    
    void setPauseMode(bool pauseMode);
    void setEditMode(bool editMode);
    
    void setEditorTool(EditorTool *tool);
    void pushEditorTool(EditorTool *tool);
    void popEditorTool();
    
    void updateDirectionalRadioButtons();

private:
    static MainWindow *_instance;
    Ui::MainWindow *_ui;
    DisplayWidget *_displayWidget;
    EditorTool *_lastTool;
    qint64 _lastFrameTime;
    QList<CharacterButton *> _characterButtons;
};

#endif // MAINWINDOW_H

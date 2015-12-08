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
    
    void onActionSize();
    void onActionOffset();

    void onActionImportTileSet();
    void onActionImport();
    void onActionExport();

    void onActionPlay();
    void onActionPause();
    void onActionEdit(bool selected);
    void onActionStep();

    void onActionToggleGrid(bool selected);
    void onCharacterButton(bool);
    
    void onTileSetSelected(int index);

public:
    void copyScreenBuffer(unsigned const char* screenBuffer, int bufferWidth, int bufferHeight);
    
public:
    inline DisplayWidget *displayWidget() const { return _displayWidget; }

protected:
    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
    void keyReleaseEvent(QKeyEvent *event) Q_DECL_OVERRIDE;

private:
    void loadTiles();
    void selectTileSet(int index);
    
    void setupToolBar(QToolBar* toolBar);
    void setupActions();
    
    void setPauseMode(bool pauseMode);
    void setEditMode(bool editMode);
    
    void setEditorTool(EditorTool *tool);
    void pushEditorTool(EditorTool *tool);
    void popEditorTool();

private:
    Ui::MainWindow *_ui;
    DisplayWidget *_displayWidget;
    EditorTool *_lastTool;
    qint64 _lastFrameTime;
    QList<CharacterButton *> _characterButtons;
};

#endif // MAINWINDOW_H

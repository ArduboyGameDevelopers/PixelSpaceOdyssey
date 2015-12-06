#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>
#include "Emulator.h"

namespace Ui {
class MainWindow;
}

class QToolBar;
class DisplayWidget;
class TilesWidget;
class EditorTool;

extern Emulator emulator;

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

    void onActionPlay();
    void onActionPause();
    void onActionEdit(bool selected);
    void onActionStep();

    void onActionToggleGrid(bool selected);

public:
    void copyScreenBuffer(unsigned const char* screenBuffer, int bufferWidth, int bufferHeight);
    
public:
    inline DisplayWidget *displayWidget() const { return _displayWidget; }

protected:
    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
    void keyReleaseEvent(QKeyEvent *event) Q_DECL_OVERRIDE;

private:
    void setupToolBar(QToolBar* toolBar);
    void setupActions();
    void setupTileSet(TilesWidget *tilesWidget);
    
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
};

#endif // MAINWINDOW_H

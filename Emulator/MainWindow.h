#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>
#include "Emulator.h"

namespace Ui {
class MainWindow;
}

class QToolBar;
class DisplayWidget;

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
    void onActionSave();

    void onActionPlay();
    void onActionPause();
    void onActionEdit(bool selected);
    void onActionStep();

    void onActionToggleGrid(bool selected);

public:
    void copyScreenBuffer(unsigned const char* screenBuffer, int bufferWidth, int bufferHeight);

protected:
    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
    void keyReleaseEvent(QKeyEvent *event) Q_DECL_OVERRIDE;

private:
    void setupToolBar(QToolBar* toolBar);
    void setupActions();
    
    void setPauseMode(bool pauseMode);
    void setEditMode(bool editMode);

private:
    Ui::MainWindow *_ui;
    DisplayWidget *_displayWidget;
    qint64 _lastFrameTime;
};

#endif // MAINWINDOW_H

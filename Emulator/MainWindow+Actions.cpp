#include "MainWindow.h"
#include "DisplayWidget.h"
#include "ui_mainwindow.h"

void MainWindow::setupActions()
{
    QAction *actionNew   = _ui->actionNew;
    QAction *actionSave  = _ui->actionSave;
    QAction *actionPlay  = _ui->actionPlay;
    QAction *actionPause = _ui->actionPause;
    QAction *actionStep  = _ui->actionStep;
    QAction *actionGrid  = _ui->actionGrid;
    QAction *actionEdit  = _ui->actionEdit;

    // set state
    setPauseMode(emulator.paused());

    // connect slots
    connect(actionNew,   SIGNAL(changed()),       this, SLOT(onActionNew()));
    connect(actionSave,  SIGNAL(changed()),       this, SLOT(onActionSave()));
    connect(actionStep,  SIGNAL(triggered()),     this, SLOT(onActionStep()));
    connect(actionPlay,  SIGNAL(triggered()),     this, SLOT(onActionPlay()));
    connect(actionPause, SIGNAL(triggered()),     this, SLOT(onActionPause()));
    connect(actionEdit,  SIGNAL(triggered(bool)), this, SLOT(onActionEdit(bool)));
    connect(actionGrid,  SIGNAL(toggled(bool)),   this, SLOT(onActionToggleGrid(bool)));
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
}

void MainWindow::onActionNew()
{
}

void MainWindow::onActionSave()
{
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
    _displayWidget->setGridVisible(selected);
}

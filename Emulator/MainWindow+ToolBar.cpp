#include "MainWindow.h"
#include "ui_mainwindow.h"

void MainWindow::setupToolBar(QToolBar *toolBar)
{
    QWidget* emptyLeft = new QWidget();
    emptyLeft->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    toolBar->insertWidget(_ui->actionPlay, emptyLeft);

    QWidget* emptyRight = new QWidget();
    emptyRight->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    toolBar->insertWidget(_ui->actionEdit, emptyRight);
}

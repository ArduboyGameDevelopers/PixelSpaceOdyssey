#include <QMessageBox>

#include "ResizeDialog.h"
#include "ui_ResizeDialog.h"

#include "Level.h"

ResizeDialog::ResizeDialog(QWidget *parent) :
    QDialog(parent),
    _ui(new Ui::ResizeDialog)
{
    _ui->setupUi(this);

    setModal(true);
}

ResizeDialog::~ResizeDialog()
{
    delete _ui;
}

void ResizeDialog::accept()
{
    bool ok;
    int top = _ui->topEdit->text().toInt(&ok);
    if (!ok)
    {
        QMessageBox::warning(NULL, "Error", "Wrong top value");
        _ui->topEdit->selectAll();
        _ui->topEdit->setFocus();
        return;
    }

    int bottom = _ui->bottomEdit->text().toInt(&ok);
    if (!ok)
    {
        QMessageBox::warning(NULL, "Error", "Wrong bottom value");
        _ui->bottomEdit->selectAll();
        _ui->bottomEdit->setFocus();
        return;
    }

    int left = _ui->leftEdit->text().toInt(&ok);
    if (!ok)
    {
        QMessageBox::warning(NULL, "Error", "Wrong left value");
        _ui->leftEdit->selectAll();
        _ui->leftEdit->setFocus();
        return;
    }

    int right = _ui->rightEdit->text().toInt(&ok);
    if (!ok)
    {
        QMessageBox::warning(NULL, "Error", "Wrong right value");
        _ui->rightEdit->selectAll();
        _ui->rightEdit->setFocus();
        return;
    }

    Level::current()->resize(top, bottom, left, right);

    QDialog::accept();
}

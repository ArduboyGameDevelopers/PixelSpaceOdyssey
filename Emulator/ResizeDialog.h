#ifndef RESIZEDIALOG_H
#define RESIZEDIALOG_H

#include <QDialog>

namespace Ui {
class ResizeDialog;
}

class ResizeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ResizeDialog(QWidget *parent = 0);
    ~ResizeDialog();

public:
    virtual void accept() Q_DECL_OVERRIDE;

private:
    Ui::ResizeDialog * _ui;
};

#endif // RESIZEDIALOG_H

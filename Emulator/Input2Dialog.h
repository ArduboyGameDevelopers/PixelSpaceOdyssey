#ifndef INPUT2DIALOG_H
#define INPUT2DIALOG_H

#include <QDialog>

#include "ui_input2dialog.h"

class Input2Dialog : public QDialog, private Ui::Input2Dialog
{
    Q_OBJECT

public:
    explicit Input2Dialog(QWidget *parent = 0, const QString &firstLabelText = QString(), const QString &secondLabelText = QString());

public:
    inline QString first() const { return firstEdit->text(); }
    inline void setFirst(const QString &first) { firstEdit->setText(first); }

    inline QString second() const { return secondEdit->text(); }
    inline void setSecond(const QString &second) { secondEdit->setText(second); }
};

#endif // INPUT2DIALOG_H

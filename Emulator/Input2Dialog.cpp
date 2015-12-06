#include "Input2Dialog.h"

Input2Dialog::Input2Dialog(QWidget *parent, const QString &firstLabelText, const QString &secondLabelText) :
    QDialog(parent)
{
    setupUi(this);

    firstLabel->setText(firstLabelText);
    secondLabel->setText(secondLabelText);

    setModal(true);
}

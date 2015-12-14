#include "ParamCheckBox.h"

ParamCheckBox::ParamCheckBox(QWidget *parent) :
    QCheckBox(parent),
    _paramPtr(NULL)
{
     connect(this, &QCheckBox::stateChanged, this, &ParamCheckBox::onStateChanged);
}

void ParamCheckBox::onStateChanged(int state)
{
    if (_paramPtr)
    {
        *_paramPtr = state == Qt::Checked;
    }
}

void ParamCheckBox::setParamPtr(bool *paramPtr)
{
    _paramPtr = paramPtr;
    
}

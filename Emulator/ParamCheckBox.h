#ifndef PARAMCHECKBOX_H
#define PARAMCHECKBOX_H

#include <QObject>
#include <QCheckBox>

class ParamCheckBox : public QCheckBox
{
    Q_OBJECT
    
public:
    explicit ParamCheckBox(QWidget *parent = 0);

public Q_SLOTS:
    void onStateChanged(int state);

public:
    inline bool *paramPtr() const { return _paramPtr; }
    void setParamPtr(bool *paramPtr);

private:
    bool *_paramPtr;
};

#endif // PARAMCHECKBOX_H

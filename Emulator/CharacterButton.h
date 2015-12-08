#ifndef CHARACTERBUTTON_H
#define CHARACTERBUTTON_H

#include <QToolButton>

#include "EditorCharTool.h"

class CharacterButton : public QToolButton
{
    Q_OBJECT
    
public:
    explicit CharacterButton(QWidget *parent = 0);
    virtual ~CharacterButton();
    
public:
    void deselectOther();
    static void deselectAll();

public:
    inline CharacterType characterType() const { return _characterType; }
    void setCharacterType(CharacterType type);

private:
    CharacterType _characterType;
};

#endif // CHARACTERBUTTON_H

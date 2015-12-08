#include "CharacterButton.h"

#include <QtGlobal>

static QList<CharacterButton *> buttons;

CharacterButton::CharacterButton(QWidget *parent) :
    QToolButton(parent)
{
    buttons.append(this);
}

CharacterButton::~CharacterButton()
{
    buttons.removeAll(this);
}

void CharacterButton::deselectOther()
{
    for (int i = 0; i < buttons.size(); ++i)
    {
        if (buttons[i] != this)
        {
            buttons[i]->setChecked(false);
        }
    }
}

void CharacterButton::deselectAll()
{
    for (int i = 0; i < buttons.size(); ++i)
    {
        buttons[i]->setChecked(false);
    }
}

void CharacterButton::setCharacterType(CharacterType type)
{
    _characterType = type;
}
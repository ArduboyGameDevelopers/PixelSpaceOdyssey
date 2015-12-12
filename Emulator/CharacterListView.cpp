#include "CharacterListView.h"

#include <QStringList>
#include <QTextStream>

static inline QString toListString(const CharacterInfo &character)
{
    return QString("%1 (%2,%3)").arg(
        character.name(),
        QString::number(character.x()),
        QString::number(character.y())
    );
}

CharacterListView::CharacterListView(QWidget *parent) :
    QListView(parent)
{
    _model = new QStringListModel(this);
    setModel(_model);
}

void CharacterListView::updateItem(Level *level)
{
    QStringList list;
    list << toListString(level->player());
 
    const QList<CharacterInfo> &enemies = level->enemies();
    for (int i = 0; i < enemies.size(); ++i)
    {
        list << toListString(enemies[i]);
    }
    
    _model->setStringList(list);
}

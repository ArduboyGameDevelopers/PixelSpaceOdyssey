#ifndef CHARACTERLISTVIEW_H
#define CHARACTERLISTVIEW_H

#include "Level.h"

#include <QListView>
#include <QStringListModel>

class CharacterListView : public QListView
{
public:
    CharacterListView(QWidget *parent = 0);
    
public:
    void updateItem(Level *level);
    void deleteItem(int index);

private:
    QStringListModel * _model;
};

#endif // CHARACTERLISTVIEW_H

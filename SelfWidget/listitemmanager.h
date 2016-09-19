#ifndef LISTITEMMANAGER_H
#define LISTITEMMANAGER_H

#include <QListWidgetItem>
#include <QObject>

#include "../actionmanager.h"

class MyListItem: public QListWidgetItem
{
public:
    MyListItem ( QListWidget * parent = 0, int type = Type);
    MyListItem ( const QString & text, QListWidget * parent = 0, int type = Type );
    MyListItem ( const QIcon & icon, const QString & text, QListWidget * parent = 0, int type = Type );
    MyListItem ( const QListWidgetItem & other );
    ~MyListItem();
};

class ListItemManager : public QObject
{
    Q_OBJECT
public:
    ListItemManager();
    static ListItemManager * instance();

    MyListItem * createListItem(const char * name,QListWidget * parent = 0);
    MyListItem * createListItem(const char * name,const QString & text, QListWidget * parent = 0, int type = QListWidgetItem::Type);
    MyListItem * createListItem(const char * name,const QIcon & icon, const QString & text, QListWidget * parent = 0, int type = QListWidgetItem::Type);
    MyListItem * createListItem(const char * name,const QListWidgetItem & other);

    bool contains(const char * name);

    ~ListItemManager();

private:
    static ListItemManager * manager;

    //qmap的key如果为自定义里类型，那么要重载<运算符
    QMap<Id,MyListItem *> listItemMap;

};

#endif // LISTITEMMANAGER_H

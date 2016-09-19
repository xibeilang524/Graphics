#include "listitemmanager.h"

MyListItem::MyListItem(QListWidget *parent, int type):
    QListWidgetItem(parent,type)
{

}

MyListItem::MyListItem(const QString &text, QListWidget *parent, int type):
    QListWidgetItem(text,parent,type)
{

}

MyListItem::MyListItem(const QIcon &icon, const QString &text, QListWidget *parent, int type):
    QListWidgetItem(icon,text,parent,type)
{

}

MyListItem::MyListItem(const QListWidgetItem &other):
    QListWidgetItem(other)
{

}

MyListItem::~MyListItem()
{

}

ListItemManager::ListItemManager()
{

}

ListItemManager * ListItemManager::manager = NULL;

ListItemManager * ListItemManager::instance()
{
    if(!manager)
    {
        manager = new ListItemManager;
    }
    return manager;
}

bool ListItemManager::contains(const char *name)
{
    QMapIterator<Id,MyListItem *> iterator(listItemMap);
    while(iterator.hasNext())
    {
        iterator.next();
        if(iterator.key().getIdName()==QString(name))
        {
            return true;
        }
    }
    return false;
}

MyListItem* ListItemManager::createListItem(const char *name, QListWidget *parent)
{
    if(contains(name))
    {
        return listItemMap.value(name);
    }
    MyListItem * item  = new MyListItem(parent);

    listItemMap.insert(Id(name),item);

    return item;
}

MyListItem* ListItemManager::createListItem(const char * name,const QString & text, QListWidget * parent, int type)
{
    if(contains(name))
    {
        return listItemMap.value(name);
    }
    MyListItem * item  = new MyListItem(text,parent,type);

    listItemMap.insert(Id(name),item);

    return item;
}

MyListItem* ListItemManager::createListItem(const char * name,const QIcon & icon, const QString & text, QListWidget * parent, int type)
{
    if(contains(name))
    {
        return listItemMap.value(name);
    }
    MyListItem * item  = new MyListItem(icon,text,parent,type);

    listItemMap.insert(Id(name),item);

    return item;
}

MyListItem* ListItemManager::createListItem(const char * name,const QListWidgetItem & other)
{
    if(contains(name))
    {
        return listItemMap.value(name);
    }
    MyListItem * item  = new MyListItem(other);

    listItemMap.insert(Id(name),item);

    return item;
}

ListItemManager::~ListItemManager()
{

}

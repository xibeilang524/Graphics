#include "menumanager.h"

#include <QDebug>

MyMenu::MyMenu(QWidget *parent):
    QMenu(parent)
{

}

MyMenu::MyMenu(const QString title, QWidget *parent):
    QMenu(title,parent)
{

}

MenuManager * MenuManager::manager = NULL;

MenuManager * MenuManager::instance()
{
    if(!manager)
    {
        manager = new MenuManager;
    }
    return manager;
}

MenuManager::MenuManager()
{

}

MyMenu * MenuManager::createMenu(const char *name, QWidget *parent)
{
    if(contains(name))
    {
        return menuMap.value(name);
    }

    MyMenu * menu  = new MyMenu(parent);
    menu->setObjectName(name);
    menuMap.insert(Id(name),menu);

    return menu;
}

MyMenu * MenuManager::createMenu(const char *name, const QString title, QWidget *parent)
{
    if(contains(name))
    {
        return menuMap.value(name);
    }

    MyMenu * menu  = new MyMenu(title,parent);
    menu->setObjectName(name);
    menuMap.insert(Id(name),menu);

    return menu;
}

bool MenuManager::contains(const char *name)
{
    QMapIterator<Id,MyMenu *> iterator(menuMap);
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

MyMenu * MenuManager::menu(const char *name)
{
    if(contains(name))
    {
        MyMenu * tmp = value(name);
        if(!tmp)
        {
            qDebug()<<"nul----";
        }
        return tmp;
    }

    return 0;
}

MyMenu * MenuManager::value(const char *name)
{
    QMapIterator<Id,MyMenu *> iterator(menuMap);
    while(iterator.hasNext())
    {
        iterator.next();
        if(iterator.key().getIdName()==QString(name))
        {
            return iterator.value();
        }
    }
    return NULL;
}

#include "actionmanager.h"

#include "string.h"

#include <QDebug>

MyAction::MyAction(QObject *parent):
    QAction(parent)
{
    type = GRA_NONE;
}

MyAction::MyAction(const QString & text,QObject *parent):
    QAction(text,parent)
{
    type = GRA_NONE;
}

MyAction::MyAction(const QIcon & icon,const QString & text,QObject *parent):
    QAction(icon,text,parent)
{
    type = GRA_NONE;
}

MyAction::~MyAction()
{

}


ActionManager::ActionManager()
{
    manager = this;
}

ActionManager * ActionManager::instance()
{
    return manager;
}

ActionManager * ActionManager::manager = NULL;


MyAction * ActionManager::crateAction(const char *name, QObject *parent)
{
    if(contains(name))
    {
        return actionMap.value(name);
    }

    MyAction * action  = new MyAction(parent);
    action->setObjectName(name);
    actionMap.insert(Id(name),action);

    return action;
}

MyAction * ActionManager::crateAction(const char *name,const QString & text,QObject *parent)
{
    if(contains(name))
    {
        return actionMap.value(name);
    }
    MyAction * action  = new MyAction(text,parent);
    action->setObjectName(name);

    actionMap.insert(Id(name),action);

    return action;
}

MyAction * ActionManager::crateAction(const char *name, const QIcon & icon, const QString & text, QObject *parent)
{
    if(contains(name))
    {
        return actionMap.value(name);
    }
    MyAction * action  = new MyAction(icon,text,parent);
    action->setObjectName(name);
    actionMap.insert(Id(name),action);

    return action;
}

MyAction * ActionManager::action(const char *name)
{
    if(contains(name))
    {
        MyAction * tmp = value(name);
        if(!tmp)
        {
            qDebug()<<"nul----";
        }
        return tmp;
    }

    return 0;
}

///*****************************************************
///**Function:
///**Description:用于判断是否包含当前名称的action
///**Input:
///**Output:
///**Return:
///**Others:
///****************************************************/
bool ActionManager::contains(const char *name)
{
    QMapIterator<Id,MyAction *> iterator(actionMap);
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

MyAction * ActionManager::value(const char *name)
{
    QMapIterator<Id,MyAction *> iterator(actionMap);
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

///*****************************************************
///**Function:
///**Description:针对不同情况注册信号和槽
///**Input:
///**Output:
///**Return:
///**Others:
///****************************************************/
bool ActionManager::registerAction(MyAction *action, QWidget *parent, const char *slot, bool isToggled)
{
    if(isToggled)
    {
        action->setCheckable(true);
        return QObject::connect(action,SIGNAL(toggled(bool)),parent,slot);
    }
    return QObject::connect(action,SIGNAL(triggered()),parent,slot);
}

ActionManager::~ActionManager()
{
    manager = NULL;

    QMapIterator<Id,MyAction *> i(actionMap);
    while (i.hasNext())
    {
        i.next();
        delete i.value();
    }
}


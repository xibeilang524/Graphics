#ifndef ACTIONMANAGER_H
#define ACTIONMANAGER_H

#include <QObject>
#include <QAction>

#include "Constants.h"
#include "Header.h"

class Id
{
public:
    Id(const char *name);

    friend bool operator<(const Id & id1,const Id & id2);
    friend bool operator==(const Id & id1,const Id & id2);

    QString getIdName()const {return this->idName;}

    ~Id();

private:
    QString idName;
};

class MyAction : public QAction
{
    Q_OBJECT
public:
    MyAction(QObject * parent);
    MyAction(const QString & text,QObject *parent);
    MyAction(const QIcon & icon,const QString & text,QObject *parent);
    void setType(GraphicsType type){this->type = type;}
    GraphicsType getType(){return this->type;}
    ~MyAction();

private:
    GraphicsType type;

};

#include <QMap>

class ActionManager : public QObject
{
    Q_OBJECT
public:
    ActionManager();
    static ActionManager * instance();
    MyAction * crateAction(const char *name, QObject * parent = 0);
    MyAction * crateAction(const char *name, const QString & text, QObject *parent = 0 );
    MyAction * crateAction(const char *name,const QIcon & icon,const QString & text,QObject *parent = 0);
    MyAction * action(const char *name);

    bool registerAction(MyAction *action,QWidget * parent,const char *slot,bool isToggled = false);
    bool contains(const char * name);

    ~ActionManager();

private:
    MyAction * value(const char *name);

    static ActionManager * manager;

    //qmap的key如果为自定义里类型，那么要重载<运算符
    QMap<Id,MyAction *> actionMap;

};

#endif // ACTIONMANAGER_H

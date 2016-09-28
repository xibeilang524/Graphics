#ifndef SUPERMANAGER_H
#define SUPERMANAGER_H

#include <QMap>

#include "id.h"

template<class T>
class SuperManager
{
public:
    bool contains(const char * name);

    //qmap的key如果为自定义里类型，那么要重载<运算符
    QMap<Id,T*> maps;
};
#endif // SUPERMANAGER_H

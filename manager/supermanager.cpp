#include "supermanager.h"

/***************Ä£°åÀà**********************/
template <class T>
bool SuperManager<T>::contains(const char *name)
{
    QMapIterator<Id,T *> iterator(maps);
    while(iterator.hasNext())
    {
        iterator.next();
        if(iterator.key().getIdName() == QString(name))
        {
            return true;
        }
    }
    return false;
}

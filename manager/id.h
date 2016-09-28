#ifndef ID_H
#define ID_H

#include <QString>

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

#endif // ID_H


/*************************************************
**版  权：RenGu Company
**文件名: abstractdataprocess.h
**作  者: wey       Version: 1.0       Date: 2016.07.12
**描  述:抽象数据操作接口
**Others:1：汇集通用数据操作方法（目前只支持单表操作，多表操作目前不支持）
**       2：新添加的数据处理需集成此接口
**
**修改历史:
**20160712:wey:实现添加、删除、更新通用访问封装
*************************************************/
#ifndef ABSTRACTDATAPROCESS_H
#define ABSTRACTDATAPROCESS_H

#include <QStringList>
#include "../Header.h"

class AbstractDataProcess
{
public:
    AbstractDataProcess();

    virtual bool insertData(const QString tableName, const QStringList insertKey, const QStringList insertValue, int &insertId);
    virtual bool insertData(const QString tableName, const QMap<QString, QString> insertKeyAndValue, int &insertId);

    virtual bool updateData(const QString tableName,const QStringList updateKey, const QStringList updateValue,
                                  const QStringList conditionKey,const QStringList conditionValue);
    virtual bool updateData(const QString tableName,const QMap<QString, QString> updateKeyAndValue,
                                  const QMap<QString, QString> conditionKeyAndValue);

    virtual bool deleteData(const QString tableName, const QStringList deleteKey,const QStringList deleteValue);
    virtual bool deleteData(const QString tableName, const QMap<QString, QString> deleteKeyAndValue);


    virtual bool getUnWrappedData(const QString tableName, const QStringList selectKey,
                                  const QStringList conditionKey, const QStringList conditionValue, DataList &result);

    virtual int getDataCount(const QString tableName);

};

#endif // ABSTRACTDATAPROCESS_H

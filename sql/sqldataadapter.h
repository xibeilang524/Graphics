/*************************************************
**版  权：RenGu Company
**文件名: sqldataadapter.h
**作  者: wey       Version: 1.0       Date: 2016.07.09
**描  述:数据库装配器
**Others:
**       1：用于接收数据使用界面传递的数据访问请求
**       2：调用数据访问层【SQLProecss】执行请求
**       3：获取数据访问层结果并定向装配到对应的结构体，交由数据使用层使用
**       4：【装配器只在查询时才调用】
**
**修改历史:
**20160709:wey:建立文件，将SQL访问部分进行合并。
**20161031:wey:增加对参数的读取
*************************************************/
#ifndef SQLDATAADAPTER_H
#define SQLDATAADAPTER_H

#include <QObject>

#include "../Header.h"

class SQLDataAdapter
{
public:
    static SQLDataAdapter * instance();

    bool getServiceData(QString sql,ServiceInfoList &list);

    bool getSericeProperties(QString sql, QList<ServiceProperty *> &list);
    bool getParameterById(QString sql,QList<Parameter> & list);

private:
    SQLDataAdapter();
    ~SQLDataAdapter();

    static SQLDataAdapter * adapter;

};

#endif // SQLDATAADAPTER_H

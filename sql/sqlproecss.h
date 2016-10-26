/*************************************************
**版  权：RenGu Company
**文件名: initdatamanager.h
**作  者: wey       Version: 1.0       Date: 2016.07.09
**描  述: 数据访问层
**Others:1：建立数据库连接
**       2：接收数据装配层【SQLDataAdapter】的数据访问请求，将执行结果返回
**       3：用于唯一与数据库访问的入口，此层不关心具体的数据语言，只执行操作。
**
**修改历史:
**20160709:wey:将访问改成单例模式
**             提供程序通用的数据库访问操作，其直接与数据装配层进行交互
**20161026:wey:增加对错误信息的保存和获取
*************************************************/
#ifndef SQLPROECSS_H
#define SQLPROECSS_H

#include "../Header.h"

class Properties;

class SQLProecss
{
public:
    static SQLProecss * instance();
    bool initDatabase(const QString dbFileName);

    int  getSqlDataCount(const QString sql);
    bool insert(const QString & sql,int & lastInserId);
    bool execute(const QString sql);
    bool obtainData(const QString sql, DataList & result);

    QString getLastError();

    bool transaction();
    bool commit();
    bool rollback();

private:
    SQLProecss();
    ~SQLProecss();

    static SQLProecss * process;

    Properties * prop;
};

#endif // SQLPROECSS_H

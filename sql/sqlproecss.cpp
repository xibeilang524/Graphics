#include "sqlproecss.h"

#include <QSqlQuery>
#include <QSqlRecord>

#include "../assisant/properties.h"
#include "../global.h"

#include <QDebug>

QSqlDatabase sqlDatabase;

SQLProecss* SQLProecss::process = NULL;

SQLProecss * SQLProecss::instance()
{
    if(!process)
    {
        process = new SQLProecss;
    }
    return process;
}

SQLProecss::SQLProecss()
{
    prop = NULL;
}

///****************`*************************************
///**Function:initDatabase
///**Description:初始化数据连接
///**Input: QString dbFileName:数据配置文件名
///**Output:
///**Return: bool : 数据配置是否成功标志
///**Others:
///****************************************************/
bool SQLProecss::initDatabase(const QString dbFileName)
{
    if(!prop)
    {
        prop = new Properties(dbFileName);
    }

    sqlDatabase = QSqlDatabase::addDatabase(prop->getValue("dataBase"));
    sqlDatabase.setHostName(prop->getValue("hostName"));
    sqlDatabase.setDatabaseName(prop->getValue("databaseName"));
    sqlDatabase.setUserName(prop->getValue("userName"));
    sqlDatabase.setPassword(prop->getValue("password"));

    return sqlDatabase.open();
}

///*****************************************************
///**Function:obtainData
///**Description:通用执行数据库访问，不对数据类型进行解析
///**Input:  QString sql : 查询条件
///**Output: DataList &result : 保存查询的数据结果
///**Return:
///**Others:由【数据装配层】调用
///****************************************************/
const bool SQLProecss::obtainData(const QString sql, DataList & result)
{
    QSqlQuery query(sqlDatabase);

//    qDebug() << __FILE__ << __FUNCTION__<<__LINE__<<__DATE__<<__TIME__<<"\n"
//             <<sql;

    if(query.exec(sql))
    {
        while(query.next())
        {
            VariantList variantList;

            for(int index = 0;  index < query.record().count();  index++ )
            {
                variantList.push_back( query.record().value(index) );
            }

            result.push_back(variantList);
        }
        return true;
    }
    return false;
}

///*****************************************************
///**Function:getSqlDataCount
///**Description:获取对应条件下数据的条数
///**Input:  const QString sql：数据执行条件
///**Output:
///**Return: const int:数据的条数
///**Others:
///****************************************************/
const int SQLProecss::getSqlDataCount(const QString sql)
{
    QSqlQuery query(sqlDatabase);
//    qDebug() << __FILE__ << __FUNCTION__<<__LINE__<<__DATE__<<__TIME__<<"\n"
//             <<sql;
    if(query.exec(sql))
    {
        if(query.next())
        {
            return query.value(0).toInt();
        }
    }
    return 0;
}

///*****************************************************
///**Function:insertData
///**Description:通用【单条】数据插入
///**Input: const QString &sql：待插入的数据语句
///**Output:
///**Return: const bool : 插入成功与否的标志
///**Others:
///****************************************************/
const bool SQLProecss::insert(const QString &sql, int &lastInserId)
{
    QSqlQuery query(sqlDatabase);
//    qDebug() << __FILE__ << __FUNCTION__<<__LINE__<<__DATE__<<__TIME__<<"\n"
//             <<sql;
    if(query.exec(sql))
    {
        lastInserId = query.lastInsertId().toInt();
        return true;
    }
    return false;
}

///*****************************************************
///**Function:execute
///**Description:执行通用的删除、更新等只需返回成功与否的语句
///**Input:const QString sql：待执行的数据语句
///**Output:无
///**Return:const bool：执行成功与否
///**Others:
///****************************************************/
const bool SQLProecss::execute(const QString sql)
{
    QSqlQuery query(sqlDatabase);
//    qDebug() << __FILE__ << __FUNCTION__<<__LINE__<<__DATE__<<__TIME__<<"\n"
//             <<sql;
    if(query.exec(sql))
    {
        return true;
    }
    return false;
}

///*****************************************************
///**Function:transaction、commit、rollback
///**Description:用于控制数据库的事物操作
///**Input:
///**Output:
///**Return:const bool : 当前操作是否成功
///**Others:
///****************************************************/
const bool SQLProecss::transaction()
{
    return sqlDatabase.transaction();
}

const bool SQLProecss::commit()
{
    return sqlDatabase.commit();
}

const bool SQLProecss::rollback()
{
    return sqlDatabase.rollback();
}

SQLProecss::~SQLProecss()
{
    if(process)
    {
        delete process;
        process = NULL;
    }

    if(prop)
    {
        delete prop;
        prop = NULL;
    }
}

#include "abstractdataprocess.h"

#include "sqlproecss.h"

#include <QDebug>

AbstractDataProcess::AbstractDataProcess()
{
}

///*****************************************************
///**Function:insertData
///**Description:通用数据库插入
///**Input:const QString tableName：待插入数据表名
///        const QStringList key：待插入字段集合
///        const QStringList value：待插入对应字段值集合
///**Output:int & insertId:返回插入成功数据库ID号
///**Return:bool:插入数据成功与否
///**Others:
///****************************************************/
bool AbstractDataProcess::insertData(const QString tableName, const QStringList insertKey,
                                           const QStringList insertValue, int & insertId)
{
    if(insertKey.size() != insertValue.size())
    {
        return false;
    }
    QString sql = "insert into "+ tableName + " ( ";

    for(int i = 0; i < insertKey.size(); i++)
    {
        sql += insertKey.at(i);
        if(i!=insertKey.size()-1)
        {
           sql += ",";
        }
    }

    sql += " ) values (";

    for(int i = 0; i < insertValue.size(); i++)
    {
        sql += insertValue.at(i);

        if(i!=insertValue.size()-1)
        {
           sql += (",");
        }
    }

    sql += " )";

    return SQLProecss::instance()->insert(sql,insertId);
}

///*****************************************************
///**Function:insertData
///**Description:通用数据库插入
///**Input:const QString tableName：待插入数据表名
///        const QMap<QString, QVariant> insertKeyAndValue：待插入字段与值集合
///**Output:int & insertId:返回插入成功数据库ID号
///**Return:bool:插入数据成功与否
///**Others:
///****************************************************/
bool AbstractDataProcess::insertData(const QString tableName, const QMap<QString, QString> insertKeyAndValue,
                                           int &insertId)
{
    return insertData(tableName,QStringList(insertKeyAndValue.keys()),
                      QStringList(insertKeyAndValue.values()),insertId);
}

///*****************************************************
///**Function:deleteData
///**Description:通用数据记录删除
///**Input:const QString tableName：待删除的数据表名
///        const QStringList deleteKey：待删除数据记录字段集合
///        const QStringList deleteValue：待删除数据记录字段值集合
///**Output:无
///**Return:bool：返回删除的成功与否
///**Others:
///****************************************************/
bool AbstractDataProcess::deleteData(const QString tableName,  const QStringList deleteKey,
                                           const QStringList deleteValue)
{
    if(deleteKey.size() != deleteValue.size())
    {
        return false;
    }
    QString sql = "delete from " + tableName + " where ";

    for(int i = 0; i < deleteKey.size();  i++)
    {
        sql += (deleteKey.at(i) + " = " + deleteValue.value(i));

        if(i != deleteKey.size()-1)
        {
            sql += " and ";
        }
    }

    return SQLProecss::instance()->execute(sql);
}

bool AbstractDataProcess::deleteData(const QString tableName, const QMap<QString, QString> deleteKeyAndValue)
{
    return deleteData(tableName,QStringList(deleteKeyAndValue.keys()),QStringList(deleteKeyAndValue.values()));
}

///*****************************************************
///**Function:updateData
///**Description:通用数据库更新
///**Input:const QString tableName：待更新数据表名
///        const QStringList key：待更新字段集合
///        const QStringList value：待更新对应字段值集合
///        const QStringList updateKey：更新的索引字段集合
///        const QStringList updateValue:更新索引对应的字段值集合
///**Output:无
///**Return:bool:更新数据成功与否
///**Others:
///****************************************************/
bool AbstractDataProcess::updateData(const QString tableName, const QStringList updateKey,
                                           const QStringList updateValue,  const QStringList conditionKey,
                                           const QStringList conditionValue)
{
    if(updateKey.size() != updateValue.size() || updateKey.size() != updateValue.size() )
    {
        return false;
    }

    QString sql = "update "+tableName +" set ";

    for(int i = 0; i<updateKey.size(); i++)
    {
        sql += (updateKey.at(i) + " = "+ updateValue.at(i));

        if(i != updateKey.size()-1)
        {
            sql += ",";
        }
    }

    sql += " where ";

    for(int i = 0; i<conditionKey.size(); i++)
    {
        sql += (conditionKey.at(i) + " = "+ conditionValue.at(i));

        if(i != conditionKey.size()-1)
        {
            sql += " and ";
        }
    }

    return SQLProecss::instance()->execute(sql);
}

bool AbstractDataProcess::updateData(const QString tableName, const QMap<QString, QString> updateKeyAndValue,
                                           const QMap<QString, QString> conditionKeyAndValue)
{
    return updateData(tableName,QStringList(updateKeyAndValue.keys()),QStringList(updateKeyAndValue.values())
                      ,QStringList(conditionKeyAndValue.keys()),QStringList(conditionKeyAndValue.values()));
}

///*****************************************************
///**Function:getDataCount
///**Description:获取对应数据表中的数据条数
///**Input:const QString tableName：数据库表名
///**Output:无
///**Return:const int：返回对应数据表的条数
///**Others:
///****************************************************/
int AbstractDataProcess::getDataCount(const QString tableName)
{
    QString condition = "select count(*) from "+tableName;

    return SQLProecss::instance()->getSqlDataCount(condition);
}

///*****************************************************
///**Function:getUnWrappedData
///**Description:数据查询，不对查询结果进行解析，交由调用者直接解析
///**Input:const QString tableName：待查询表名
///        const QStringList selectKey：待查询的字段
///        const QStringList conditionKey:待查询条件
///        const QStringList conditionValue：待查询条件对应值
///**Output:DataList & result：查询结果
///**Return:const bool：执行成功或者失败
///**Others:
///****************************************************/
bool AbstractDataProcess::getUnWrappedData(const QString tableName, const QStringList selectKey,
                                                 const QStringList conditionKey, const QStringList conditionValue,
                                                 DataList & result)
{
    if(conditionKey.size() != conditionValue.size())
    {
        return false;
    }

    QString sql = " select ";
    if(selectKey.size()==0)
    {
        sql += " *";
    }
    else
    {
        for(int i = 0; i < selectKey.size(); i++)
        {
            sql += selectKey.at(i);
            if(i != selectKey.size()-1)
            {
                sql += " , ";
            }
        }
    }

    sql +=(" from " + tableName);

    if(conditionKey.size() > 0)
    {
        sql += " where ";

        for(int i = 0; i < conditionKey.size(); i++)
        {
            sql += ( conditionKey.at(i) + " = " +conditionValue.at(i));
            if(i != conditionKey.size()-1)
            {
                sql += " , ";
            }
        }
    }

    return SQLProecss::instance()->obtainData(sql,result);
}



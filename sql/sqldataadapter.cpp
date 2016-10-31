#include "sqldataadapter.h"

#include "sqlproecss.h"

SQLDataAdapter * SQLDataAdapter::adapter = NULL;


SQLDataAdapter::SQLDataAdapter()
{

}

SQLDataAdapter* SQLDataAdapter::instance()
{
    if(!adapter)
    {
        adapter = new SQLDataAdapter;
    }
    return adapter;
}

//获取服务的基本信息，并进行装配
bool SQLDataAdapter::getServiceData(QString sql, ServiceInfoList &list)
{
    DataList dataList;
    bool flag = SQLProecss::instance()->obtainData(sql,dataList);
    foreach (VariantList variant, dataList)
    {
        ServiceInfo  info;
        info.id = variant.at(0).toString();
        info.code = variant.at(1).toString();
        info.name = variant.at(2).toString();
        info.descirption = variant.at(3).toString();
        info.status = variant.at(4).toString();
        info.auditOpinion = variant.at(5).toString();
        info.method = variant.at(6).toString();
        info.input = variant.at(7).toString();
        info.result = variant.at(8).toString();
        info.num = variant.at(9).toInt();
        info.addTime = variant.at(10).toDateTime();
        info.addUser = variant.at(11).toString();
        info.auditUser = variant.at(12).toString();
        info.auditUserName = variant.at(13).toString();
        info.outputName = variant.at(14).toString();
        info.outputType = variant.at(15).toString();
        info.addUserName = variant.at(16).toString();
        info.auditTime = variant.at(17).toDateTime();
        list.push_back(info);
    }
    return flag;
}

//获取所有的属性信息，将结果保存至全局的列表中
bool SQLDataAdapter::getSericeProperties(QString sql, QList<ServiceProperty *> &list)
{
    DataList dataList;
    bool flag = SQLProecss::instance()->obtainData(sql,dataList);
    foreach (VariantList variant, dataList)
    {
        ServiceProperty * property = new ServiceProperty;
        property->id = variant.at(0).toString();
        property->serviceName = variant.at(1).toString();
        property->status = variant.at(2).toString();
        property->descirption = variant.at(3).toString();
        property->servicePath = variant.at(4).toString();
        property->method = variant.at(5).toString();

        Parameter * output = new Parameter;
        output->pName = variant.at(6).toString();
        output->pType = variant.at(7).toString();
        property->outputParas.append(output);

        list.append(property);
    }
    return flag;
}

//根据Id查找对应服务的参数
bool SQLDataAdapter::getParameterById(QString sql,QList<Parameter> & list)
{
    DataList dataList;
    bool flag = SQLProecss::instance()->obtainData(sql,dataList);
    foreach (VariantList variant, dataList)
    {
        Parameter para;
        para.pId = variant.at(0).toString();
        para.pName = variant.at(1).toString();
        para.pType = variant.at(2).toString();
        para.pRemark = variant.at(3).toDateTime().toString("yyyy-MM-dd");
        list.append(para);
    }
    return flag;
}

SQLDataAdapter::~SQLDataAdapter()
{
    if(adapter)
    {
        delete adapter;
        adapter = NULL;
    }
}

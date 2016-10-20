#include "serviceinfoprocess.h"

#include "sqldataadapter.h"

ServiceInfoProcess * ServiceInfoProcess::serviceProcess = NULL;

ServiceInfoProcess::ServiceInfoProcess()
{
}

ServiceInfoProcess * ServiceInfoProcess::instance()
{
    if(!serviceProcess)
    {
        serviceProcess = new ServiceInfoProcess;
    }
    return serviceProcess;
}

//分页查询服务的基本信息
void ServiceInfoProcess::getServiceInfo(ServiceInfoList &list, const int startIndex, const int perPage)
{
    QString sql = "select * from business_softwareonline ";
    if(perPage != 0)
    {
        sql += (" limit ")+QString::number(startIndex) +","+QString::number(perPage);
    }

    SQLDataAdapter::instance()->getServiceData(sql,list);
}

//获取所有的属性信息
void ServiceInfoProcess::getServiceProperty(QList<ServiceProperty> &properties)
{
    QString sql = "select s.name,s.status,s.descirption,sd.url,s.method from\
            business_softwareonline s left join business_softwareonline_deploy sd\
                on s.id = sd.relationId";

   SQLDataAdapter::instance()->getSericeProperties(sql,properties);
}

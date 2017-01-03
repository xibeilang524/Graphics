/*************************************************
**版  权：RenGu Company
**文件名: serviceinfoprocess.h
**作  者: wey       Version: 1.0       Date: 2016.10.18
**描  述:
**Others: 服务信息访问
**
**修改历史:
**20170103:wey:添加根据服务名查找对应的URL
*************************************************/
#ifndef SERVICEINFOPROCESS_H
#define SERVICEINFOPROCESS_H

#include "abstractdataprocess.h"
#include "../Header.h"

class ServiceInfoProcess : public AbstractDataProcess
{
public:
    static ServiceInfoProcess * instance();
    bool getServiceInfo(ServiceInfoList & list,const int startIndex = 0,const int perPage = 0);

    bool getServiceProperty(QList<ServiceProperty *> &properties);

    QString getLastError();

    bool setServiceStatus(QString id, bool isEnable);

    bool getUrlByServiceName(QString sname, QString &result);

private:
    ServiceInfoProcess();
    static ServiceInfoProcess * serviceProcess;

};

#endif // SERVICEINFOPROCESS_H

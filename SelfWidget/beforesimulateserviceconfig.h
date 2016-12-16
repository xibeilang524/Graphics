/*************************************************
**版  权：RenGu Company
**文件名: beforesimulateserviceconfig.h
**作  者: wey       Version: 1.0       Date: 2016.12.16
**描  述: 服务推演前统一服务配置模块
**Others:在流程检查无误，并且在推演之前，弹出此框用于用于对
**       所有输入信息的确认，允许编辑和修改。
**
**修改历史:
**
*************************************************/
#ifndef BEFORESIMULATESERVICECONFIG_H
#define BEFORESIMULATESERVICECONFIG_H

#include <QDialog>

namespace Ui {
class BeforeSimulateServiceConfig;
}
class QListWidgetItem;
class MyChooseBar;
class MyItem;
class ServiceInputTableView;

struct ProcessUnit;
struct ServiceProperty;

class BeforeSimulateServiceConfig : public QDialog
{
    Q_OBJECT
    
public:
    explicit BeforeSimulateServiceConfig(QWidget *parent = 0);
    ~BeforeSimulateServiceConfig();

    void setProcedureData(QList<ProcessUnit *> & procUnits);

private slots:
    void respSwitchService(int currIndex);
    void switchServiceInfo(int currIndex);
    void applyService();
    
private:
    void initServiceData();
    void findHasExistedService(ServiceProperty * prop);
    void updateDelegateList(QList<MyItem *> pItems);
    void updateLeftQuoteTable(QList<MyItem *> & pItems);

    Ui::BeforeSimulateServiceConfig *ui;

    MyChooseBar * chooseBar;
    ServiceInputTableView * inputTableView;
    ServiceInputTableView * outputTableView;

    QList<ProcessUnit *>  procUnits;        //只保存处理过程
    bool isComboxAutoChanged;               //下拉框如果自动改变，则无需响应槽函数
    int currServiceIndex;                   //当前服务索引
};

#endif // BEFORESIMULATESERVICECONFIG_H

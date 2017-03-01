/*************************************************
**版  权：RenGu Company
**文件名: initservice.h
**作  者: wey       Version: 1.0       Date: 2016.12.19
**描  述: 初始化服务的执行环境
**Others:
**
**修改历史:
**2017-02-28:添加将预处理和恢复环境流程保存至本地磁盘
*************************************************/
#ifndef INITSERVICE_H
#define INITSERVICE_H

#include <QDialog>

namespace Ui {
class InitService;
}

class QListWidgetItem;
class MyChooseBar;
class MyItem;
class ServiceInputTableView;

struct ProcessUnit;
struct ServiceProperty;

class InitService : public QDialog
{
    Q_OBJECT
    
public:
    explicit InitService(QWidget *parent = 0);
    ~InitService();

    void preService();
    void resetService();

private slots:
    void respSwitchService(QListWidgetItem *item);
    void switchServiceInfo(int currIndex);
    void addCurrService();
    void removeCurrService();
    void applyUpdateService();
    void saveToDisk();
    void loadFromDisk();
    
private:
    void initServiceData();
    void updateDelegateList(QList<MyItem *> pItems);
    bool findHasExistedService(QString sname);
    void showSelectedItemInfo(ServiceProperty *prop);
    void setButtState(bool isAddEnabled);

    Ui::InitService *ui;

    MyChooseBar * chooseBar;
    ServiceInputTableView * inputTableView;
    ServiceInputTableView * outputTableView;

    bool isComboxAutoChanged;               //下拉框如果自动改变，则无需响应槽函数
    bool isPreModel;                        //是否为预处理模式
    int currServiceIndex;                   //listwidget中显示的索引
};

#endif // INITSERVICE_H

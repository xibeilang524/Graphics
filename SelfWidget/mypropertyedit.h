/*************************************************
**版  权：RenGu Company
**文件名: mypropertyedit.h
**作  者: wey       Version: 1.0       Date: 2016.10.03
**描  述:
**Others: 控件属性编辑窗口
**
**修改历史:
**20160120:wey:修改服务的属性信息，增加从数据库读取信息
**20161123:wey:调整服务引用由一个输出至可引用多个输出中的一个
**20161125:wey:调整输出参数自动解析，支持多个参数输出。
*************************************************/
#ifndef MYPROPERTYEDIT_H
#define MYPROPERTYEDIT_H

#include <QDialog>

class ServiceInputTableView;
class MyItem;
class MyChooseBar;
struct ServiceProperty;

namespace Ui {
class MyPropertyEdit;
}

class MyPropertyEdit : public QDialog
{
    Q_OBJECT
    
public:
    explicit MyPropertyEdit(QWidget *parent = 0);
    ~MyPropertyEdit();

    void initProp(ServiceProperty * prop, bool isEditable = true);
    void updateDelegateList(QList<MyItem *> pItems);

private slots:
    void confirmPropety();
    void switchServiceInfo(int index);
    
private:
    void initServiceData();
    void updateLeftQuoteTable(QList<MyItem *> & pItems);

    Ui::MyPropertyEdit *ui;

    ServiceInputTableView * inputTableView;
    ServiceInputTableView * outputTableView;

    ServiceProperty * currItemProp;

    bool isComboxAutoChanged;               //下拉框如果自动改变，则无需响应槽函数
    MyChooseBar * chooseBar;
    bool isEditState;                       //是否为编辑状态
};

#endif // MYPROPERTYEDIT_H

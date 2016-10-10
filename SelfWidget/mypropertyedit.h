/*************************************************
**版  权：RenGu Company
**文件名: mypropertyedit.h
**作  者: wey       Version: 1.0       Date: 2016.10.03
**描  述:
**Others: 控件属性编辑窗口
**
**修改历史:
**
*************************************************/
#ifndef MYPROPERTYEDIT_H
#define MYPROPERTYEDIT_H

#include <QDialog>

class ServiceInputTableView;
class ServiceProperty;

namespace Ui {
class MyPropertyEdit;
}

class MyPropertyEdit : public QDialog
{
    Q_OBJECT
    
public:
    explicit MyPropertyEdit(QWidget *parent = 0);
    ~MyPropertyEdit();

    void initProp(ServiceProperty * prop);

private slots:
    void confirmPropety();
    void cancelProperty();
    
private:
    Ui::MyPropertyEdit *ui;

    ServiceInputTableView * inputTableView;
    ServiceInputTableView * outputTableView;

    ServiceProperty * currItemProp;
};

#endif // MYPROPERTYEDIT_H

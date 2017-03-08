/*************************************************
**版  权：RenGu Company
**文件名: mystatemodeldialog.h
**作  者: wey       Version: 1.0       Date: 2016.11.08
**描  述:模型块状态设置
**Others:
**
**修改历史:
**20170308:wey:增加对表格等操作。
*************************************************/
#ifndef MYSTATEMODELDIALOG_H
#define MYSTATEMODELDIALOG_H

#include <QDialog>
#include <QModelIndex>
class MyChooseBar;

#include "../Header.h"

namespace Ui {
class MyStateModelDialog;
}

class MyStateModelDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit MyStateModelDialog(QWidget *parent = 0);
    ~MyStateModelDialog();

    void setModelProp(StateModelProperty &);
    StateModelProperty & getModelProp(){return this->prop;}
    
private slots:
    void respContinueAction();
    void respAddItem();
    void respRemoveAllItem();
    void respRemoveSelectedItem();
    void updateInfo();
    void respShowCurrItem(QModelIndex index);

private:
    void addRow(StatInnerProperty & pp);

    Ui::MyStateModelDialog *ui;
    MyChooseBar * chooseBar;

    StateModelProperty prop;                //保存原始的数据
    QList<StatInnerProperty> props;         //保存最新的的条目集合

    QString continueContent;
};

#endif // MYSTATEMODELDIALOG_H

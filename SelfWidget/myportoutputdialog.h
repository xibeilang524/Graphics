/*************************************************
**版  权：RenGu Company
**文件名: myportoutputdialog.h
**作  者: wey       Version: 1.0       Date: 2016.11.08
**描  述: 输出端口设置
**Others:
**
**修改历史:
**
*************************************************/
#ifndef MYPORTOUTPUTDIALOG_H
#define MYPORTOUTPUTDIALOG_H

#include <QDialog>
#include <QModelIndex>

#include "../Header.h"

class MyChooseBar;
class QTableWidgetItem;

namespace Ui {
class MyPortOutputDialog;
}

class MyPortOutputDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit MyPortOutputDialog(QWidget *parent = 0);
    ~MyPortOutputDialog();

    void setInOutState(bool isInstate = true);
    void setProp(StateInOutProperty & prop);
    StateInOutProperty & getProp();

private slots:
    void updateProp();
    void addNewItem();
    void removeSelectedItem();
    void removeAllItem();
    void respDClickItem(QModelIndex);

private:
    void addRow(const QString pname, const QString ptype);

    Ui::MyPortOutputDialog *ui;

    StateInOutProperty sprop;
    QList<StatePortProperty> props;         //保存暂存的添加集合
    MyChooseBar * chooseBar;
};

#endif // MYPORTOUTPUTDIALOG_H

/*************************************************
**版  权：RenGu Company
**文件名: MyStateSetDialog.h
**作  者: wey       Version: 1.0       Date: 2016.11.08
**描  述: 状态设置
**Others:
**
**修改历史:
**
*************************************************/
#ifndef MYSTATESETDIALOG_H
#define MYSTATESETDIALOG_H

#include <QDialog>

#include "Header.h"

class MyChooseBar;

namespace Ui {
class MyStateSetDialog;
}

class MyStateSetDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit MyStateSetDialog(QWidget *parent = 0);
    ~MyStateSetDialog();

    void initComboxList(QStringList & innerList,QStringList & outerList);
    void setProp(LinkedStateProperty & linkProp);
    LinkedStateProperty & getProp(){return this->prop;}

private slots:
    void respSetInterEventList(bool);
    void respSetOuterEventList(bool);
    void respConfirm();
    
private:
    Ui::MyStateSetDialog *ui;
    MyChooseBar * chooseBar;

    LinkedStateProperty  prop;

    QStringList innerList;
    QStringList outerList;
};

#endif // MYSTATESETDIALOG_H

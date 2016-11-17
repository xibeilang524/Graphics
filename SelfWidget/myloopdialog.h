/*************************************************
**版  权：RenGu Company
**文件名: myloopdialog.h
**作  者: wey       Version: 1.0       Date: 2016.11.11
**描  述: 循环框输入条件
**Others:
**
**修改历史:
**
*************************************************/
#ifndef MYLOOPDIALOG_H
#define MYLOOPDIALOG_H

#include <QDialog>

class ParameterDefineTableView;
class MyItem;
class MyChooseBar;
struct LoopProperty;

namespace Ui {
class MyLoopDialog;
}

class MyLoopDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit MyLoopDialog(QWidget *parent = 0);
    ~MyLoopDialog();

    void setLoopItemProp(LoopProperty * prop);

private slots:
    void respButtYesPress();
    void respVariEdited();
    void respDeleteVari(QString name);
    
private:
    Ui::MyLoopDialog *ui;

    ParameterDefineTableView * express1View;
    ParameterDefineTableView * express2View;
    ParameterDefineTableView * express3View;

    LoopProperty * loopProp;
    MyChooseBar * chooseBar;
};

#endif // MYLOOPDIALOG_H

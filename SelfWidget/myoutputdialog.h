/*************************************************
**版  权：RenGu Company
**文件名: myoutputdialog.h
**作  者: wey       Version: 1.0       Date: 2016.11.30
**描  述: 输入输出框
**Others:用于显示输出的信息，可以引用前面的参数
**
**修改历史:
**
*************************************************/
#ifndef MYOUTPUTDIALOG_H
#define MYOUTPUTDIALOG_H

#include <QDialog>

class MyChooseBar;
class MyPlainTextEdit;
class MyItem;
class QListWidgetItem;
struct JudgeProperty;

namespace Ui {
class MyOutputDialog;
}

class MyOutputDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit MyOutputDialog(QWidget *parent = 0);
    ~MyOutputDialog();

    void setOutput(MyItem * item, bool isEditable = true);

private slots:
    void respButtYesPress();
    void addQuote(QListWidgetItem* item);
    
private:
    Ui::MyOutputDialog *ui;

    MyPlainTextEdit * plainEdit;
    MyChooseBar * chooseBar;

    JudgeProperty * jprop;

    bool isEditState;               //是否为可编辑状态
};

#endif // MYOUTPUTDIALOG_H

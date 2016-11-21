/*************************************************
**版  权：RenGu Company
**文件名: myconditionsetting.h
**作  者: wey       Version: 1.0       Date: 2016.10.28
**描  述:
**Others:条件输入框
**
**修改历史:
**20161121:wey:增加条件表达式转换后结果显示
*************************************************/
#ifndef MYCONDITIONSETTING_H
#define MYCONDITIONSETTING_H

#include <QDialog>

class MyChooseBar;
class QListWidgetItem;
class MyItem;
class MyPlainTextEdit;
struct JudgeProperty;

namespace Ui {
class MyConditionSetting;
}

class MyConditionSetting : public QDialog
{
    Q_OBJECT
    
public:
    explicit MyConditionSetting(QWidget *parent = 0);
    ~MyConditionSetting();

    void setJudgeProp(MyItem *item);

private slots:
    void addQuote(QListWidgetItem *item);
    void respConfirmPressed();
    
private:
    Ui::MyConditionSetting *ui;

    MyPlainTextEdit * plainEdit;
    MyChooseBar * chooseBar;
    JudgeProperty * jprop;
};

#endif // MYCONDITIONSETTING_H

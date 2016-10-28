/*************************************************
**版  权：RenGu Company
**文件名: myconditionsetting.h
**作  者: wey       Version: 1.0       Date: 2016.10.28
**描  述:
**Others:条件输入框
**
**修改历史:
**
*************************************************/
#ifndef MYCONDITIONSETTING_H
#define MYCONDITIONSETTING_H

#include <QDialog>

namespace Ui {
class MyConditionSetting;
}

class MyConditionSetting : public QDialog
{
    Q_OBJECT
    
public:
    explicit MyConditionSetting(QWidget *parent = 0);
    ~MyConditionSetting();
    
private:
    Ui::MyConditionSetting *ui;
};

#endif // MYCONDITIONSETTING_H

/*************************************************
**版  权：RenGu Company
**文件名: myfinaldescpage.h
**作  者: wey       Version: 1.0       Date: 2016.11.09
**描  述: 创建工程汇总显示页
**Others:
**
**修改历史:
**
*************************************************/
#ifndef MYFINALDESCPAGE_H
#define MYFINALDESCPAGE_H

#include <QWizardPage>

namespace Ui {
class MyFinalDescPage;
}

class MyFinalDescPage : public QWizardPage
{
    Q_OBJECT
    
public:
    explicit MyFinalDescPage(QWidget *parent = 0);
    ~MyFinalDescPage();

protected:
    void initializePage();
    
private:
    Ui::MyFinalDescPage *ui;
};

#endif // MYFINALDESCPAGE_H

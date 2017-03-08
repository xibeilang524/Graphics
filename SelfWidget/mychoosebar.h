/*************************************************
**版  权：RenGu Company
**文件名: mychoosebar.h
**作  者: wey       Version: 1.0       Date: 2016.11.08
**描  述: yes/no选择框
**Others:
**
**修改历史:
**
*************************************************/
#ifndef MYCHOOSEBAR_H
#define MYCHOOSEBAR_H

#include <QWidget>

namespace Ui {
class MyChooseBar;
}

class MyChooseBar : public QWidget
{
    Q_OBJECT
    
public:
    explicit MyChooseBar(QWidget *parent = 0);
    ~MyChooseBar();

    void setParetWidget(QWidget * parent);

signals:
    void confirmPressed();
    void cancelPressed();

private slots:
    void respCancel();
    void respConfirm();
    
private:
    Ui::MyChooseBar *ui;

    QWidget * pWidget;
};

#endif // MYCHOOSEBAR_H

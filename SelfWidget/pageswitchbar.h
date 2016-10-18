/*************************************************
**版  权：RenGu Company
**文件名: pageswitchbar.h
**作  者: wey       Version: 1.0       Date: 2016.07.08
**描  述: 页面切换工具栏
**Others:
**      1：用于统一所有页面中的页面切换控件
**
**修改历史:
**
*************************************************/
#ifndef PAGESWITCHBAR_H
#define PAGESWITCHBAR_H

#include <QWidget>

class PageInfo;

namespace Ui {
class PageSwitchBar;
}

class PageSwitchBar : public QWidget
{
    Q_OBJECT
    
public:
    explicit PageSwitchBar(QWidget *parent = 0);
    void showTipInfo();
    void setPerPageShow(const int num);
    void setTotalNum(const int num);
    void setCurrPage(const int num);
    const int getTotalNums();
    const int getCurrPage();
    const int getPerPageShow();
    const int getTotalPage();
    QString getFormatOutput(const QString & format ="/" );
    ~PageSwitchBar();

signals:
    void signalBeginSearch();
    void signalCloseDialog();
    void signalInserData();

public slots:
    void showMe(){}
    void hideMe(){}

private slots:
    void respSwitchPage();
    void respSearch();
    void respPerPageChange(int);
    
private:
    void setPageShowNum();

    void initConnection();

    Ui::PageSwitchBar *ui;

    PageInfo * pageInfo;                       //切换页
};

#endif // PAGESWITCHBAR_H

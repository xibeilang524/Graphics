/*************************************************
**版  权：RenGu Company
**文件名: serviceview.h
**作  者: wey       Version: 1.0       Date: 2016.10.18
**描  述:
**Others:查看服务的信息
**
**修改历史:
**
*************************************************/
#ifndef SERVICEVIEW_H
#define SERVICEVIEW_H

#include <QDialog>

#include "../Header.h"

class PageSwitchBar;
class QTableWidget;

namespace Ui {
class ServiceView;
}

class ServiceView : public QDialog
{
    Q_OBJECT
    
public:
    static ServiceView * instance();
    ~ServiceView();

private slots:
    void viewDatabaseContent();
    void refreshDatabaseContent();
    void beginSearch();                     //开始查询

private:
    void initComponent();
    void resizeTable(QTableWidget * table);

    explicit ServiceView(QWidget *parent = 0);
    static ServiceView * serviceView;

    Ui::ServiceView *ui;

    PageSwitchBar * pageBar;

    RowList rowIndexs;

};

#endif // SERVICEVIEW_H

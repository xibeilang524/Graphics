#include "serviceview.h"
#include "ui_serviceview.h"

#include <QMessageBox>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QDebug>

#include "../global.h"
#include "../util.h"
#include "../sql/serviceinfoprocess.h"
#include "pageswitchbar.h"

#define MAX_ROW_COUNT 10

ServiceView * ServiceView::serviceView = NULL;

ServiceView * ServiceView::instance()
{
    if(!serviceView)
    {
        serviceView = new ServiceView(GlobalMainWindow);
    }

    return serviceView;
}

ServiceView::ServiceView(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ServiceView)
{
    ui->setupUi(this);

    setWindowTitle("服务概览信息");

    int width = 950;
    int height = 600;

    setGeometry((ScreenWidth - width)/2,(ScreenHeight - height)/2,width,height);

    initComponent();
}

//初始化控件
void ServiceView::initComponent()
{
    pageBar = new PageSwitchBar(ui->widget_3);
    pageBar->setPerPageShow(MAX_ROW_COUNT);

    connect(pageBar,SIGNAL(signalBeginSearch()),this,SLOT(beginSearch()));

    QHBoxLayout * barLayout = new QHBoxLayout;
    barLayout->addWidget(pageBar);
    barLayout->setContentsMargins(1,1,1,1);
    ui->widget_3->setLayout(barLayout);

    resizeTable(ui->tableWidget);
    Util::setTableStylesheet(ui->tableWidget);
}

//响应控件栏查询命令
void ServiceView::beginSearch()
{
    QString tableName = "business_softwareonline";
    int envirCount = ServiceInfoProcess::instance()->getDataCount(tableName);

    pageBar->setTotalNum(envirCount);
    pageBar->showTipInfo();

    ServiceInfoList list;
    int startIndex = pageBar->getCurrPage()*pageBar->getPerPageShow();
    ServiceInfoProcess::instance()->getServiceInfo(list,startIndex,pageBar->getPerPageShow());

    rowIndexs.clear();

    resizeTable(ui->tableWidget);

    for(int i = 0; i < list.size();  i++)
    {
        ServiceInfo info = list.at(i);
        ui->tableWidget->item(i,0)->setText(info.id);
        ui->tableWidget->item(i,1)->setText(info.descirption);
        ui->tableWidget->item(i,2)->setText(info.status);
        ui->tableWidget->item(i,3)->setText(info.method);
        ui->tableWidget->item(i,4)->setText(info.addTime.toString("yyyy-MM-dd hh:mm:ss"));
        ui->tableWidget->item(i,5)->setText(info.addUser);

        RowToIndex rx;
        rx.indexNum = list.at(i).id;
        rx.rowNum = i;
        rowIndexs.push_back(rx);
    }
}

//如果当前表格的行数和每页显示的数目不一致，则需要将表格重新创建
//如果一致则只需将表格内容清空
void ServiceView::resizeTable(QTableWidget * table)
{
    table->show();
    if(pageBar->getPerPageShow()!=table->rowCount())
    {
        Util::removeTableItem(table);

        table->setColumnWidth(0,260);
        table->setColumnWidth(1,120);
        table->setColumnWidth(2,100);
        table->setColumnWidth(3,150);
        table->setColumnWidth(4,120);
        table->setColumnWidth(5,100);

        Util::createTableItem(pageBar->getPerPageShow(),table);
    }
    else
    {
        Util::clearTableData(table);
    }
}

//查看数据库信息
void ServiceView::viewDatabaseContent()
{
    exec();
}

//刷新数据库信息
void ServiceView::refreshDatabaseContent()
{
    GlobalServiceProperties.clear();
    ServiceInfoProcess::instance()->getServiceProperty(GlobalServiceProperties);

    QMessageBox::information(this,"提示","数据库更新完毕");
}

ServiceView::~ServiceView()
{
    delete ui;
}

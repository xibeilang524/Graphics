#include "serviceinputtableview.h"

#include "serviceinputmodel.h"
#include "serviceinputdelegate.h"

#include <QMenu>
#include <QAction>
#include <QContextMenuEvent>
#include <QHeaderView>
#include <QDebug>

ServiceInputTableView::ServiceInputTableView(int row, int column, QWidget *parent):
    QTableView(parent)
{
    serviceModel = new ServiceInputModel(row,column);
    serviceDelegate = new ServiceInputDelegate;

    setModel(serviceModel);
    setItemDelegate(serviceDelegate);

    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::SingleSelection);

    horizontalHeader()->setStretchLastSection(true);

    //设置表头宽度
    QHeaderView * view = new QHeaderView(Qt::Horizontal,this);
    view->setResizeMode(QHeaderView::Custom);
    setHorizontalHeader(view);

    view->resizeSection(2,200);
    view->setStretchLastSection(true);

    initContextMenu();
}

//返回当前的模型
ServiceInputModel * ServiceInputTableView::model()const
{
    return serviceModel;
}

//初始化右键菜单
void ServiceInputTableView::initContextMenu()
{
    rightMenu = new QMenu();
    insertAction = new QAction("插入行",rightMenu);
    deleteAction = new QAction("删除行",rightMenu);

    connect(insertAction,SIGNAL(triggered()),this,SLOT(insertRow()));
    connect(deleteAction,SIGNAL(triggered()),this,SLOT(deleteRow()));

    rightMenu->addAction(insertAction);
    rightMenu->addAction(deleteAction);
}

//单行插入
void ServiceInputTableView::insertRow()
{
    serviceModel->addRow();
}

//单行插入输入参数
void ServiceInputTableView::insertRow(Parameter * intputPara)
{
    serviceModel->addRow(intputPara);
}

//单行删除
void ServiceInputTableView::deleteRow()
{
    if(selectedIndexes().size() > 0)
    {
        int selectedRow = selectedIndexes().at(0).row();
        serviceModel->deleteRow(selectedRow);
    }
}

//清空表格
void ServiceInputTableView::clearTable()
{
    serviceModel->clearTable();
}

void ServiceInputTableView::contextMenuEvent(QContextMenuEvent *)
{
//    rightMenu->exec(event->globalPos());
}

ServiceInputDelegate * ServiceInputTableView::delegate()
{
    return this->serviceDelegate;
}

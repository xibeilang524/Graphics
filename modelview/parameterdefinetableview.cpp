#include "parameterdefinetableview.h"

#include "parameterdefinemodel.h"
#include "parameterdefinedelegate.h"

#include <QMenu>
#include <QAction>
#include <QContextMenuEvent>
#include <QHeaderView>
#include <QDebug>

ParameterDefineTableView::ParameterDefineTableView(LoopPart loopPart, int row, int column, QWidget *parent):
    QTableView(parent)
{
    paraModel = new ParameterDefineModel(loopPart,row,column);
    paraDelegate = new ParameterDefineDelegate(loopPart);

    connect(paraModel,SIGNAL(variNumEdited()),this,SIGNAL(variContentEdited()));
    connect(paraModel,SIGNAL(deleteVariInfo(QString)),this,SIGNAL(deleteVari(QString)));

    setModel(paraModel);
    setItemDelegate(paraDelegate);

    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::SingleSelection);

    horizontalHeader()->setStretchLastSection(true);

    //设置表头宽度
    QHeaderView * view = new QHeaderView(Qt::Horizontal,this);
    view->setResizeMode(QHeaderView::Custom);
    setHorizontalHeader(view);

    view->setStretchLastSection(true);

    if(loopPart == LOOP_VARI)
    {
        view->resizeSection(1,200);
    }
    else
    {
        view->resizeSection(0,200);
    }

    initContextMenu();
}

//初始化右键菜单
void ParameterDefineTableView::initContextMenu()
{
    rightMenu = new QMenu();
    insertAction = new QAction("插入行",rightMenu);
    deleteAction = new QAction("删除行",rightMenu);

    connect(insertAction,SIGNAL(triggered()),this,SLOT(insertRow()));
    connect(deleteAction,SIGNAL(triggered()),this,SLOT(deleteRow()));

    rightMenu->addAction(insertAction);
    rightMenu->addAction(deleteAction);
}

void ParameterDefineTableView::contextMenuEvent(QContextMenuEvent *event)
{
    rightMenu->exec(event->globalPos());
}

//单行插入
void ParameterDefineTableView::insertRow()
{
    paraModel->addRow();
}

//单行插入输入参数
//void ParameterDefineTableView::insertRow(Parameter * intputPara)
//{
//    serviceModel->addRow(intputPara);
//}

//单行删除
void ParameterDefineTableView::deleteRow()
{
    if(selectedIndexes().size() > 0)
    {
        int selectedRow = selectedIndexes().at(0).row();
        paraModel->deleteRow(selectedRow);
    }
}

void ParameterDefineTableView::setLoopItemProp(LoopProperty *prop)
{
    paraModel->setParameterProp(prop);
}

//返回当前的模型
ParameterDefineModel * ParameterDefineTableView::model()const
{
    return paraModel;
}

//返回当前的模型
ParameterDefineDelegate * ParameterDefineTableView::delegate()
{
    return paraDelegate;
}

ParameterDefineTableView::~ParameterDefineTableView()
{
    delete paraDelegate;
    delete paraModel;
}

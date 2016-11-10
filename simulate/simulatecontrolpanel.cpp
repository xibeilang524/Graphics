#include "simulatecontrolpanel.h"
#include "ui_simulatecontrolpanel.h"

#include <QDebug>
#include <QApplication>
#include <QMessageBox>

#include "SimulateHeader.h"
#include "sceneitempickup.h"
#include "processcheck.h"
#include "simulateutil.h"

#include "../item/myitem.h"
#include "../item/mygraphicsview.h"
#include "../item/myscene.h"
#include "../util.h"
#include "../global.h"
#include "../webservice/mywebservice.h"

MyListWidgetItem::MyListWidgetItem(QListWidget *parent, int type)
    :QListWidgetItem(parent,type)
{
    punit = NULL;
}

MyListWidgetItem::MyListWidgetItem(const QString &text, QListWidget *parent, int type)
    :QListWidgetItem(text,parent,type)
{
    punit = NULL;
}

//绑定处理单元，用于反向的控制
void MyListWidgetItem::bindProscessUnit(ProcessUnit *unit)
{
    this->punit = unit;
}

SimulateControlPanel::SimulateControlPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SimulateControlPanel)
{
    ui->setupUi(this);

    setFixedWidth(300);
    currProcUnit = NULL;

    connect(ui->startSimulate,SIGNAL(clicked()),this,SLOT(respStartSimulate()));
    connect(ui->simProcedure,SIGNAL(currentItemChanged(QListWidgetItem * , QListWidgetItem *)),this,SLOT(respItemChanged(QListWidgetItem * , QListWidgetItem *)));
    connect(ui->simProcedure,SIGNAL(itemClicked (QListWidgetItem*)),this,SLOT(respItemActivated(QListWidgetItem*)));
    connect(ui->simProcedure,SIGNAL(itemDoubleClicked(QListWidgetItem *)),this,SLOT(respItemDoubleClicked(QListWidgetItem *)));
    connect(this,SIGNAL(sendSingleSimulate(ProcessUnit*)),this,SLOT(showSimulateOperate(ProcessUnit*)));

    connect(MyWebService::instance(),SIGNAL(lastUnitProcessOver(bool,QString)),this,SLOT(procLastUnitResult(bool,QString)));
}

/*!
 *开始流程推演
 *【1】:提取当前场景中控件和线条
 *【2】:判断是否包含开始和结束
 *【3】:判断整个链路是否完整，同时判断逻辑是否正确；并且将控件按照逻辑顺序重新排列【重要】
 *【4】:从头开始提取控件，将其转换成处理单元，压入处理队列【重要】
 *【5】:对队列进行处理，同时对当前处理的控件进行高亮标识【重要】
 *!*/
void SimulateControlPanel::respStartSimulate()
{
    setSimulateState(true);

    emit resetSimluate();
    resetSimluateFlag();

    QList<QGraphicsItem *> existedItems;
    //【1】【2】
    ReturnType returnType = SceneItemPickup::instance()->pickUpItems(existedItems);

    if(returnType != RETURN_SUCCESS)
    {
        setFlagState(ui->step1,false);
        setSimulateState(false);
        return;
    }
    else
    {
        setFlagState(ui->step1,true);
    }

    //【3】【4】
    QList<MyItem *> resortedItems;

    returnType = ProcessCheck::instance()->checkProcess(existedItems,resortedItems,procUnits);

    if(returnType != RETURN_SUCCESS)
    {
        setFlagState(ui->step2,false);
        setSimulateState(false);
        return;
    }
    else
    {
        setFlagState(ui->step2,true);
        setFlagState(ui->step3,true);
    }

    qDebug()<<"============================";

    foreach(ProcessUnit * tmpUnit,procUnits)
    {
        if(tmpUnit->gtype == GRA_POLYGON)
        {
            qDebug()<<tmpUnit->item->getText()<<"__"<<tmpUnit->yesChild->item->getText()<<"_"<<tmpUnit->noChild->item->getText();
        }
        else
        {
            if(tmpUnit->nextChild)
            {
                qDebug()<<tmpUnit->item->getText()<<"__"<<tmpUnit->nextChild->item->getText();
            }
            else
            {
                qDebug()<<tmpUnit->item->getText();
            }

        }
    }


    //【5】对处理单元进行处理
    currProcUnit = procUnits.first();
    startProcUnit();

    setSimulateState(false);
}

//针对当前处理单元返回的结果进行处理
void SimulateControlPanel::procLastUnitResult(bool hasFault,QString context)
{
    if(hasFault)
    {
        currProcUnit->item->hightLightItem(LEVEL_MIDDLE,true);
        Util::showWarn("服务访问出错，推演流程终止!");
        return;
    }

    currProcUnit->item->hightLightItem(LEVEL_HIGH,true);
    if(currProcUnit->ptype == PRO_PROCESS)
    {
        ServiceProperty * prop = currProcUnit->item->getServiceProp();
        Parameter * para = prop->outputParas.at(0);

        para->pValue = context;

        currProcUnit = currProcUnit->nextChild;
    }
    //判断框
    else if(currProcUnit->ptype == PRO_JUDGE)
    {

    }
    //循环框
    else if(currProcUnit->ptype == PRO_LOOP)
    {

    }
    //结束
    else if(currProcUnit->ptype == PRO_END)
    {

    }

    startProcUnit();
}

//开始处理单元
void SimulateControlPanel::startProcUnit()
{
    while(true)
    {
        if(currProcUnit->ptype == PRO_START)
        {
            currProcUnit->item->hightLightItem(LEVEL_HIGH,true);
            emit sendSingleSimulate(currProcUnit);
            currProcUnit = currProcUnit->nextChild;
        }
        //处理框
        else if(currProcUnit->ptype == PRO_PROCESS)
        {
            ServiceProperty * prop = currProcUnit->item->getServiceProp();
            submitUrl(currProcUnit->item,prop);
            emit sendSingleSimulate(currProcUnit);
            break;
        }
        //判断框
        else if(currProcUnit->ptype == PRO_JUDGE)
        {
break;
        }
        //循环框
        else if(currProcUnit->ptype == PRO_LOOP)
        {
break;
        }
        //结束
        else if(currProcUnit->ptype == PRO_END)
        {
            emit sendSingleSimulate(currProcUnit);
            currProcUnit->item->hightLightItem(LEVEL_HIGH,true);
            break;
        }
    }
}

//组装URL值
void SimulateControlPanel::submitUrl(MyItem * item, ServiceProperty * prop)
{
    QString fullUrl = prop->servicePath+"/"+prop->method;

    if(prop->inputParas.size() > 0)
    {
        fullUrl += "?";
        for(int i = 0;i<prop->inputParas.size();i++)
        {
            QString paraValue = prop->inputParas.at(i)->pValue;
            QString realParaValue;
            if(paraValue.startsWith(QString(COMBOX_START_FLAG)))
            {
                realParaValue = getQuoteOutValue(item,paraValue);
            }
            else
            {
                realParaValue = prop->inputParas.at(i)->pValue;
            }

            fullUrl += (prop->inputParas.at(i)->pName+"="+ realParaValue);

            if(i != prop->inputParas.size() - 1)
            {
                fullUrl += "&";
            }
        }
    }

//    qDebug()<<fullUrl;

    MyWebService::instance()->submit(fullUrl);
}

//根据引用的参数来反向查所引用的输出值
QString SimulateControlPanel::getQuoteOutValue(MyItem * item,QString value)
{
   QList<MyItem *> parentItems =  SimulateUtil::instance()->getCurrParentItem(item);
   int pos = value.indexOf("]");
   int startLen = QString(COMBOX_START_FLAG).size();
   int parentIndex = value.mid(startLen,pos - startLen).toInt();

   if(parentIndex >= 0 && parentIndex < parentItems.size())
   {
       ServiceProperty * sp = parentItems.at(parentIndex)->getServiceProp();
       ParaList out = sp->outputParas;

       if(out.size() > 0 )
       {
           return out.at(0)->pValue;
       }
   }

   return "0";
}

//根据是否为推演状态设置控件的状态
void SimulateControlPanel::setSimulateState(bool isSim)
{
    GlobalIsSimulateState = isSim;
    ui->startSimulate->setEnabled(!isSim);
}

//显示每一步的推演的操作信息
void SimulateControlPanel::showSimulateOperate(ProcessUnit *unit)
{
    MY_ASSERT(unit)
    QPixmap pixmap;

    Util::loadPixmapByGType(unit->gtype,pixmap);

    MyListWidgetItem * item = new MyListWidgetItem;
    item->setIcon(QIcon(pixmap));
    item->setText(unit->item->getText());
    item->bindProscessUnit(unit);

    ui->simProcedure->addItem(item);
}

//点击控件时，高亮显示
void SimulateControlPanel::respItemActivated(QListWidgetItem * current)
{
    MY_ASSERT(current)

    MyListWidgetItem * citem = dynamic_cast<MyListWidgetItem *>(current);
    if(citem)
    {
        citem->getUnit()->item->hightLightItem(LEVEL_HIGH,true);

        SimulateUtil::instance()->getCurrParentItem(citem->getUnit()->item);
    }
}

//根据选择的item，反向控制控件
void SimulateControlPanel::respItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    Q_UNUSED(current)
    MY_ASSERT(previous)

    MyListWidgetItem * pitem = dynamic_cast<MyListWidgetItem *>(previous);

    if(pitem)
    {
        pitem->getUnit()->item->hightLightItem(LEVEL_MIDDLE,true);
    }

    MyGraphicsView::instance()->update();
}

//双击右侧的列表，可以查看当前的属性
void SimulateControlPanel::respItemDoubleClicked(QListWidgetItem *current)
{
    MY_ASSERT(current)
    MyListWidgetItem * citem = dynamic_cast<MyListWidgetItem *>(current);

    if(citem)
    {
        MyGraphicsView::instance()->showSelectedItemPropEdit(citem->getUnit()->item);
//        QEvent * event(QEvent::GraphicsSceneContextMenu);
//        MyGraphicsView::instance()->scene()->sendEvent(citem->getUnit()->item,event);
    }
}

//重置推演流程验证提示
void SimulateControlPanel::resetSimluateFlag()
{
    ui->step1->setStyleSheet("");
    ui->step2->setStyleSheet("");
    ui->step3->setStyleSheet("");

    ui->simProcedure->clear();

    foreach(ProcessUnit * unit,procUnits)
    {
        delete unit;
    }
    procUnits.clear();
}

//根据检查的结果设置流程中样式
void SimulateControlPanel::setFlagState(QLabel * label,bool isSuccess)
{
    if(isSuccess)
    {
        label->setStyleSheet("background-color:green;color:white;border-radius:4px;");
    }
    else
    {
        label->setStyleSheet("background-color:red;color:white;border-radius:4px;");
    }
}

SimulateControlPanel::~SimulateControlPanel()
{
    delete ui;
}

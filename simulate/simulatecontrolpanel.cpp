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
    isSimulateState = false;
    isAutoRun = true;

    connect(ui->startSimulate,SIGNAL(clicked()),this,SLOT(respStartSimulate()));
    connect(ui->simProcedure,SIGNAL(currentItemChanged(QListWidgetItem * , QListWidgetItem *)),this,SLOT(respItemChanged(QListWidgetItem * , QListWidgetItem *)));
    connect(ui->simProcedure,SIGNAL(itemClicked (QListWidgetItem*)),this,SLOT(respItemActivated(QListWidgetItem*)));
    connect(ui->simProcedure,SIGNAL(itemDoubleClicked(QListWidgetItem *)),this,SLOT(respItemDoubleClicked(QListWidgetItem *)));
    connect(ui->terminalSimulate,SIGNAL(clicked()),this,SLOT(stopCurrSimulate()));
    connect(this,SIGNAL(sendSingleSimulate(ProcessUnit*)),this,SLOT(showSimulateOperate(ProcessUnit*)));

    connect(ui->autoRun,SIGNAL(toggled(bool)),this,SLOT(chooseRunMethod(bool)));
    connect(ui->signalRun,SIGNAL(toggled(bool)),this,SLOT(chooseRunMethod(bool)));
    connect(ui->nextStep,SIGNAL(clicked()),this,SLOT(stepByStep()));

    ui->autoRun->setChecked(true);
    ui->terminalSimulate->setEnabled(false);

    connect(MyWebService::instance(),SIGNAL(lastUnitProcessOver(bool,QString)),this,SLOT(procLastUnitResult(bool,QString)));
}

//自动运行
void SimulateControlPanel::chooseRunMethod(bool flag)
{
    QString objName = QObject::sender()->objectName();

    if(objName == "autoRun")
    {
        if(flag)
        {
            isAutoRun = true;
            ui->nextStep->setEnabled(false);
        }
    }
    else if(objName == "signalRun")
    {
        if(flag)
        {
            isAutoRun = false;
            ui->nextStep->setEnabled(true);
        }
    }
}

//下一步
void SimulateControlPanel::stepByStep()
{
    if(isSimulateState)
    {
        startProcUnit();
    }
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
        tmpUnit->item->setProcessType(tmpUnit->ptype);

        if(tmpUnit->gtype == GRA_POLYGON)
        {
            qDebug()<<tmpUnit->ptype<<"_"<<tmpUnit->item->getText()<<"__"<<tmpUnit->yesChild->item->getText()<<"_"<<tmpUnit->noChild->item->getText();
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

    clearLastSimluteRecord();

    isSimulateState = true;
    //【5】对处理单元进行处理
    currProcUnit = procUnits.first();
    startProcUnit();   
}

//清空上一次推演的记录
void SimulateControlPanel::clearLastSimluteRecord()
{
    //再次推演时先清空上一次的记录
    foreach(ProcessUnit * unit,procUnits)
    {
        if(unit->ptype == PRO_LOOP )
        {
            LoopProperty * lprop = unit->item->getLoopProp();
            foreach(SignalVari  * tmp ,lprop->signalList)
            {
                tmp->middlValue = tmp->initialValue;
            }
        }
    }
}

//针对当前处理单元返回的结果进行处理
void SimulateControlPanel::procLastUnitResult(bool hasFault,QString context)
{
    if(hasFault)
    {
        currProcUnit->item->hightLightItem(LEVEL_MIDDLE,true);
        Util::showWarn("服务访问出错，推演流程终止!");
        isSimulateState = false;
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

    if(isAutoRun)
    {
        startProcUnit();
    }
}

//开始处理单元
void SimulateControlPanel::startProcUnit()
{
    while(isSimulateState)
    {
        //在右侧模拟面板中加入当前推演控件
        emit sendSingleSimulate(currProcUnit);

        if(currProcUnit->ptype == PRO_START)
        {
            currProcUnit->item->hightLightItem(LEVEL_HIGH,true);
            currProcUnit = currProcUnit->nextChild;
        }
        //处理框
        else if(currProcUnit->ptype == PRO_PROCESS)
        {
            ServiceProperty * prop = currProcUnit->item->getServiceProp();
            submitUrl(currProcUnit->item,prop);

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
            currProcUnit->item->hightLightItem(LEVEL_HIGH,true);

            LoopProperty * loopProp = currProcUnit->item->getLoopProp();
            SignalVariList slist = loopProp->signalList;

            bool loopResult = countLoopValue(slist);

            if(loopResult)
            { 
                currProcUnit = currProcUnit->yesChild;
            }
            else
            {
                //在执行否时，需要将上次循环的中间值置为0(嵌套循环)
                currProcUnit = currProcUnit->noChild;
                foreach(SignalVari * tmpVari,slist)
                {
                    tmpVari->middlValue = tmpVari->initialValue;
                }
            }
        }
        //结束
        else if(currProcUnit->ptype == PRO_END)
        {
            currProcUnit->item->hightLightItem(LEVEL_HIGH,true);
            setSimulateState(false);
            break;
        }

        if(!isAutoRun)
        {
            break;
        }
    }
}

//根据循环框中设定的值，计算当前条件是否满足；若满足，执行循环框yes分支，否则执行no分支
bool SimulateControlPanel::countLoopValue(SignalVariList &loopList)
{
    if(loopList.size() == 0)
    {
        return false;
    }

    bool finalResult = false;

    //先判断条件是否成立
    foreach(SignalVari * sv,loopList)
    {
        if(sv->operateSymbol == "<")
        {
            if(sv->middlValue < sv->finalValue)
            {
                finalResult = true;
            }
        }
        else if(sv->operateSymbol == "<=")
        {
            if(sv->middlValue <= sv->finalValue)
            {
                finalResult = true;
            }
        }
        else if(sv->operateSymbol == ">")
        {
            if(sv->middlValue > sv->finalValue)
            {
                finalResult = true;
            }
        }
        else if(sv->operateSymbol == ">=")
        {
            if(sv->middlValue >= sv->finalValue)
            {
                finalResult = true;
            }
        }
        else if(sv->operateSymbol == "==")
        {
            if(sv->middlValue == sv->finalValue)
            {
                finalResult = true;
            }
        }
    }

    //条件成立，对变量进行改变操作
    if(finalResult)
    {
        foreach(SignalVari * sv,loopList)
        {
            int middval = sv->middlValue;

            if(sv->selfOperateSymbol == "++")
            {
                sv->middlValue = middval+1;
            }
            else if(sv->selfOperateSymbol == "--")
            {
                sv->middlValue = middval-1;
            }
            else if(sv->selfOperateSymbol == "+=")
            {
                sv->middlValue = middval + sv->selfOperateValue;
            }
            else if(sv->selfOperateSymbol == "-=")
            {
                sv->middlValue = middval - sv->selfOperateValue;
            }
        }
    }

    return finalResult;
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

//终止当前的推演
void SimulateControlPanel::stopCurrSimulate()
{
    isSimulateState = false;
    setSimulateState(false);
}

//根据是否为推演状态设置控件的状态
void SimulateControlPanel::setSimulateState(bool isSim)
{
    isSimulateState = isSim;
    GlobalIsSimulateState = isSim;
    ui->startSimulate->setEnabled(!isSim);
    ui->terminalSimulate->setEnabled(isSim);

    if(isSim)
    {
        ui->autoRun->setEnabled(false);
        ui->signalRun->setEnabled(false);
    }
    else
    {
        if(isAutoRun)
        {
            ui->autoRun->setEnabled(true);
            ui->signalRun->setEnabled(true);
            ui->nextStep->setEnabled(false);
        }
        else
        {
            ui->autoRun->setEnabled(true);
            ui->signalRun->setEnabled(true);
            ui->nextStep->setEnabled(true);
        }
    }
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
    ui->simProcedure->scrollToBottom();
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

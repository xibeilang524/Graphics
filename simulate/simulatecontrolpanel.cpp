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
    beforeUnit = NULL;
    isSimulateState = false;
    isAutoRun = true;

    connect(ui->startSimulate,SIGNAL(clicked()),this,SLOT(respStartSimulate()));
    connect(ui->simProcedure,SIGNAL(currentItemChanged(QListWidgetItem * , QListWidgetItem *)),this,SLOT(respItemChanged(QListWidgetItem * , QListWidgetItem *)));
    connect(ui->simProcedure,SIGNAL(itemClicked (QListWidgetItem*)),this,SLOT(respItemActivated(QListWidgetItem*)));
    connect(ui->simProcedure,SIGNAL(itemDoubleClicked(QListWidgetItem *)),this,SLOT(respItemDoubleClicked(QListWidgetItem *)));
    connect(ui->terminalSimulate,SIGNAL(clicked()),this,SLOT(stopCurrSimulate()));

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
    foreach(ProcessUnit * unit,procUnits)
    {
        if(unit->ptype == PRO_LOOP)
        {
            LoopProperty * lprop = unit->item->getLoopProp();
            foreach(SignalVari  * tmp ,lprop->signalList)
            {
                tmp->isAssignedValue = false;
                tmp->middlValue = tmp->initialValue;
                tmp->middleResults.clear();
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
        setSimulateState(false);
        return;
    }

    if(currProcUnit->ptype == PRO_PROCESS)
    {
        ServiceProperty * prop = currProcUnit->item->getServiceProp();
        Parameter * para = prop->outputParas.at(0);

        para->pValue = context;

        currProcUnit = currProcUnit->nextChild;
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
        MyListWidgetItem * currListItem = showSimulateOperate(currProcUnit);

        if(currProcUnit->ptype == PRO_START)
        {
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
            JudgeProperty * jprop = currProcUnit->item->getJudegeProp();

            bool judgeResult = countJudgeValue(currProcUnit->item,jprop->express);
            if(judgeResult)
            {
                currProcUnit = currProcUnit->yesChild;
            }
            else
            {
                currProcUnit = currProcUnit->noChild;
            }
        }
        //循环框
        else if(currProcUnit->ptype == PRO_LOOP)
        {
            int loopRecord = 0;
            bool isExisted = false;
            foreach(LoopRecord  * lr,loopRecords)
            {
                if(lr->item == currProcUnit->item)
                {
                    lr->record+=1;
                    loopRecord = lr->record;
                    isExisted = true;
                    break;
                }
            }

            if(!isExisted)
            {
                LoopRecord  * lr = new LoopRecord;
                lr->item = currProcUnit->item;
                lr->record = 0;
                loopRecord = lr->record;
                loopRecords.push_back(lr);
            }
            if(currListItem)
            {
                currListItem->setData(Qt::UserRole,loopRecord);
            }

            LoopProperty * loopProp = currProcUnit->item->getLoopProp();
            SignalVariList slist = loopProp->signalList;

            bool loopResult = countLoopValue(currProcUnit->item,slist);

            if(loopResult)
            {
                currProcUnit = currProcUnit->yesChild;
            }
            else
            {
                //在执行否时，需要将上次循环的中间值置为0(嵌套循环)
                foreach(SignalVari * tmpVari,slist)
                {
                    tmpVari->middlValue = tmpVari->initialValue;
                }
                int index = 0;
                for(int i = 0; i < loopRecords.size(); i++)
                {
                    if(loopRecords.at(i)->item == currProcUnit->item)
                    {
                        index = i;
                        delete loopRecords.at(i);
                        break;
                    }
                }
                loopRecords.removeAt(index);

                currProcUnit = currProcUnit->noChild;
            }
        }
        //结束
        else if(currProcUnit->ptype == PRO_END)
        {
            setSimulateState(false);
            currProcUnit->item->hightLightItem(LEVEL_NORMAL,true);
            break;
        }

        if(!isAutoRun)
        {
            break;
        }
    }
}

/*!根据循环框中设定的值，计算当前条件是否满足；若满足，执行循环框yes分支，否则执行no分支
 *20161117:添加循环中引用前面服务计算出的结果值
 *         添加对循环次数的记录，将其保存至备注信息中
 **/
bool SimulateControlPanel::countLoopValue(MyItem * item, SignalVariList &loopList)
{
    if(loopList.size() == 0)
    {
        return false;
    }

    bool finalResult = false;

    //判断循环条件中是否包含引用，如果包含则需要对引用的值进行获取
    foreach(SignalVari * sv,loopList)
    {
        if(sv->isQuoted && !sv->isAssignedValue)
        {
            QList<MyItem *> pItems = SimulateUtil::instance()->getCurrParentItem(item);
            int index = 1;
            foreach(MyItem * tmpItem,pItems)
            {
                ServiceProperty * prop = tmpItem->getServiceProp();

                if(prop->outputParas.size() == 1)
                {
                    QString newItem = QString(COMBOX_START_FLAG)+QString::number(index)+"]"+tmpItem->getText()+":"+prop->outputParas.at(0)->pName;
                    index++;
                    if(newItem == sv->variName)
                    {
                        sv->initialValue = prop->outputParas.at(0)->pValue.toInt();
                        sv->middlValue = sv->initialValue;
                        sv->isAssignedValue = true;
                        break;
                    }
                }
            }
        }
    }

    //判断条件是否成立
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

    //记录每次循环过程中产生变量的中间值
    foreach(SignalVari * sv,loopList)
    {
        sv->middleResults.append(sv->middlValue);
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

   if(parentIndex >= 1 && parentIndex <= parentItems.size())
   {
       ServiceProperty * sp = parentItems.at(parentIndex - 1)->getServiceProp();
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
    beforeUnit = NULL;

    foreach(LoopRecord * lr,loopRecords)
    {
        delete lr;
    }
    loopRecords.clear();

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
MyListWidgetItem * SimulateControlPanel::showSimulateOperate(ProcessUnit *unit)
{
    MY_ASSERT_RETURN_NULL(unit)
    QPixmap pixmap;

    if(beforeUnit)
    {
       beforeUnit->item->hightLightItem(LEVEL_NORMAL,true);
    }
    unit->item->hightLightItem(LEVEL_HIGH,true);

    beforeUnit = unit;

    Util::loadPixmapByGType(unit->gtype,pixmap);

    MyListWidgetItem * item = new MyListWidgetItem;
    item->setIcon(QIcon(pixmap));
    item->setText(unit->item->getText());
    item->bindProscessUnit(unit);

    ui->simProcedure->addItem(item);
    ui->simProcedure->scrollToBottom();

    return item;
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
        pitem->getUnit()->item->hightLightItem(LEVEL_NORMAL,true);
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
        MyGraphicsView::instance()->showSelectedItemPropEdit(citem->getUnit()->item,citem->data(Qt::UserRole));
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

/*!针对逻辑表达式求值,目前不支持表达式里调用方法
  【1】对表达式中出现的变量或者引用进行替换
  【2】对表达式正确性验证(暂未考虑)
  【3】对输入的字符进行词法分析，将一个字符串按照类别进行提取
  【4】对表达式整体进行逻辑处理，返回计算的结果
!*/
bool SimulateControlPanel::countJudgeValue(MyItem * item, QString express)
{
    //【1】
    QString switchResult = switchQuoteParameter(item,express);
    item->getJudegeProp()->switchExpress = switchResult;

    qDebug() <<__FUNCTION__
            <<switchResult<<"++++++++++"<<express
            <<"\n";

    //【2】：无

    //【3】
    QStringList result ;
    bool flag = SimulateUtil::instance()->parseText(switchResult,result);
    if(flag)
    {
        //【4】
        qDebug()<<result;
    }
    else
    {
        Util::showWarn("判断条件解析出错，请检查判断条件!");
        return false;
    }
    return true;
}

//对表达式中的引用参数进行替换，中括号要使用"\\["
QString SimulateControlPanel::switchQuoteParameter(MyItem * item,QString & express)
{
    QString finalResult = express;
    QRegExp exp("(\\[([a-zA-Z]+):(\\d{1}):([a-zA-Z]+)\\])");

    QList<QString> quoteResults;
    int pos = 0;
    while((pos = exp.indexIn(express,pos))>=0)
    {
        QString quoteName = exp.cap(3);
        QString result = findQuoteResult(item,quoteName);
        quoteResults.append(result);
        pos += exp.matchedLength();
    }

    foreach(QString result,quoteResults)
    {
        int firstIndex = exp.indexIn(finalResult);
        if(firstIndex >= 0)
        {
            finalResult.replace(firstIndex,exp.matchedLength(),result);
        }
    }

    return finalResult;
}

//针对引用的名称，查找其对应的输出结果值
QString SimulateControlPanel::findQuoteResult(MyItem * item,QString quoteName)
{
    QList<MyItem *> parentItems =  SimulateUtil::instance()->getCurrParentItem(item);
    foreach(MyItem * item,parentItems)
    {
        if(item->getText() == quoteName)
        {
            ParaList out = item->getServiceProp()->outputParas;
            if(out.size() > 0 )
            {
                return out.at(0)->pValue;
            }
        }
    }

    return "0";
}

SimulateControlPanel::~SimulateControlPanel()
{
    delete ui;
}

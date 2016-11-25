#include "myloopdialog.h"
#include "ui_myloopdialog.h"

#include "../global.h"
#include "../Header.h"
#include "../modelview/parameterdefinetableview.h"
#include "../modelview/parameterdefinemodel.h"
#include "../modelview/parameterdefinedelegate.h"
#include "mychoosebar.h"

#include <QDebug>

MyLoopDialog::MyLoopDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MyLoopDialog)
{
    ui->setupUi(this);

    setWindowTitle("循环条件编辑");

    setGeometry((ScreenWidth - POP_SIMULATE_DIALOG_WIDTH)/2,(ScreenHeight - POP_SIMULATE_DIALOG_HEIGHT)/2,POP_SIMULATE_DIALOG_WIDTH,POP_SIMULATE_DIALOG_HEIGHT);

    chooseBar = new MyChooseBar(ui->widget_2);
    chooseBar->setParetWidget(this);
    connect(chooseBar,SIGNAL(confirmPressed()),this,SLOT(respButtYesPress()));

    QHBoxLayout * layout = new QHBoxLayout;
    layout->addWidget(chooseBar);
    layout->setContentsMargins(1,1,1,1);
    ui->widget_2->setLayout(layout);

    express1View = new ParameterDefineTableView(LOOP_VARI,0,4,ui->express1);
    express2View = new ParameterDefineTableView(LOOP_EXPRESS,0,4,ui->express1);
    express3View = new ParameterDefineTableView(LOOP_FINAL,0,4,ui->express1);

    connect(express1View,SIGNAL(variContentEdited()),this,SLOT(respVariEdited()));
    connect(express1View,SIGNAL(deleteVari(QString)),this,SLOT(respDeleteVari(QString)));

    QHBoxLayout * layout1 = new QHBoxLayout;
    layout1->addWidget(express1View);
    layout1->setContentsMargins(2,2,2,2);
    ui->express1->setLayout(layout1);

    QHBoxLayout * layout2 = new QHBoxLayout;
    layout2->addWidget(express2View);
    layout2->setContentsMargins(2,2,2,2);
    ui->express2->setLayout(layout2);

    QHBoxLayout * layout3 = new QHBoxLayout;
    layout3->addWidget(express3View);
    layout3->setContentsMargins(2,2,2,2);
    ui->express3->setLayout(layout3);
}

void MyLoopDialog::setLoopItemProp(LoopProperty *prop,QVariant userRoleData,bool isEditable)
{
    this->loopProp = prop;
    isEditState = isEditable;

    express1View->setLoopItemProp(prop,userRoleData);
    express2View->setLoopItemProp(prop,userRoleData);
    express3View->setLoopItemProp(prop,userRoleData);

    express1View->setEnabled(isEditable);
    express2View->setEnabled(isEditable);
    express3View->setEnabled(isEditable);

    respVariEdited();
}

//点击确定后，对设置的属性进行保存;同时对产生的判断条件进行组装
void MyLoopDialog::respButtYesPress()
{
    MY_ASSERT(loopProp)

    if(!isEditState)
    {
        return;
    }

    foreach(VariableDefine * tmp,loopProp->varList)
    {
        delete tmp;
    }

    loopProp->varList.clear();

    foreach(ExpressDefine * tmp,loopProp->expList)
    {
        delete tmp;
    }

    loopProp->expList.clear();

    foreach(FinalExpressDefine * tmp,loopProp->fexpList)
    {
        delete tmp;
    }

    loopProp->fexpList.clear();

    foreach(SignalVari * tmpValue,loopProp->signalList)
    {
        delete tmpValue;
    }

    loopProp->signalList.clear();

    foreach (VariableDefine * tmp, express1View->model()->getPara1())
    {
        VariableDefine * newVari = new VariableDefine;
        newVari->name = tmp->name;
        newVari->type = tmp->type;
        newVari->value = tmp->value;
        newVari->pRemark = tmp->pRemark;
        loopProp->varList.push_back(newVari);
    }

    foreach(ExpressDefine * tmp,express2View->model()->getPara2())
    {
        ExpressDefine * newExp = new ExpressDefine;
        newExp->name = tmp->name;
        newExp->expressType = tmp->expressType;
        newExp->value = tmp->value;
        newExp->pRemark = tmp->pRemark;
        loopProp->expList.push_back(newExp);
    }

    foreach(FinalExpressDefine * tmp,express3View->model()->getPara3())
    {
        FinalExpressDefine * newFexp = new FinalExpressDefine;
        newFexp->name = tmp->name;
        newFexp->expressType = tmp->expressType;
        newFexp->value = tmp->value;
        newFexp->pRemark = tmp->pRemark;
        loopProp->fexpList.push_back(newFexp);
    }

    if(loopProp->expList.size() != loopProp->fexpList.size())
    {
        return;
    }

    //设置变量名
    for(int i = 0; i < loopProp->varList.size(); i++)
    {
        SignalVari * svari = new SignalVari;
        if(loopProp->varList.at(i)->type == QString(COMBOX_LOOP_QUOTE))
        {
            svari->isQuoted = true;
            svari->variName = loopProp->varList.at(i)->name;
        }
        else
        {
            svari->variName = loopProp->varList.at(i)->name;
            svari->initialValue = loopProp->varList.at(i)->value.toInt();
            svari->middlValue = svari->initialValue;
        }
        loopProp->signalList.append(svari);
    }

    //设置变量的结束条件
    for(int i = 0; i < loopProp->expList.size(); i++)
    {
        for(int j = 0; j < loopProp->signalList.size(); j++)
        {
            if(loopProp->expList.at(i)->name == loopProp->signalList.at(j)->variName)
            {
                loopProp->signalList.at(j)->finalValue = loopProp->expList.at(i)->value.toInt();
                loopProp->signalList.at(j)->operateSymbol = loopProp->expList.at(i)->expressType;
                break;
            }
        }
    }

    //设置变量变化条件
    for(int i = 0; i < loopProp->fexpList.size(); i++)
    {
        for(int j = 0; j < loopProp->signalList.size(); j++)
        {
            if(loopProp->fexpList.at(i)->name == loopProp->signalList.at(j)->variName)
            {
                loopProp->signalList.at(j)->selfOperateSymbol = loopProp->fexpList.at(i)->expressType;
                loopProp->signalList.at(j)->selfOperateValue = loopProp->fexpList.at(i)->value.toInt();
                break;
            }
        }
    }

}

//当变量列表被编辑后，那么需要更新另外两者delegate的列表
void MyLoopDialog::respVariEdited()
{
    VariableDefineList list = express1View->model()->getPara1();
    QStringList currVarList;
    foreach(VariableDefine * tmpItem,list)
    {
        currVarList<<tmpItem->name;
    }

    express2View->delegate()->updateVariList(currVarList);
    express3View->delegate()->updateVariList(currVarList);
}

//删除变量表后，自动删除另外两张
void MyLoopDialog::respDeleteVari(QString name)
{
    express2View->model()->deleteRow(name);
    express3View->model()->deleteRow(name);
}

MyLoopDialog::~MyLoopDialog()
{
    delete express1View;
    delete express2View;
    delete express3View;
    delete ui;
}

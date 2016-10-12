#include "simulatecontrolpanel.h"
#include "ui_simulatecontrolpanel.h"

#include <QDebug>
#include <QMessageBox>

#include "SimulateHeader.h"
#include "sceneitempickup.h"
#include "processcheck.h"

SimulateControlPanel::SimulateControlPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SimulateControlPanel)
{
    ui->setupUi(this);

    setFixedWidth(300);

    connect(ui->startSimulate,SIGNAL(clicked()),this,SLOT(respStartSimulate()));
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
    QList<QGraphicsItem *> existedItems;
    //【1】【2】
    ReturnType returnType = SceneItemPickup::instance()->pickUpItems(existedItems);

    if(returnType != RETURN_SUCCESS)
    {
        QMessageBox::information(0,"提示",QString("%1").arg(returnType));
        return;
    }
    else
    {
        QMessageBox::information(0,"提示","起始验证正确");
    }

    //【3】
    returnType = ProcessCheck::instance()->checkProcess(existedItems);

    if(returnType != RETURN_SUCCESS)
    {
        QMessageBox::information(0,"提示",QString("%1").arg(returnType));
        return;
    }
    else
    {
        QMessageBox::information(0,"提示","流程验证正确");
    }


}

SimulateControlPanel::~SimulateControlPanel()
{
    delete ui;
}

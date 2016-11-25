#include "myconditionsetting.h"
#include "ui_myconditionsetting.h"

#include <QHBoxLayout>
#include <QTextCursor>
#include <QDebug>

#include "../global.h"
#include "../Header.h"
#include "../manager/listitemmanager.h"
#include "../simulate/simulateutil.h"
#include "../item/mygraphicsview.h"
#include "../item/myitem.h"
#include "myplaintextedit.h"
#include "mychoosebar.h"

MyConditionSetting::MyConditionSetting(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MyConditionSetting)
{
    ui->setupUi(this);

    setWindowTitle("判断条件编辑");

    setGeometry((ScreenWidth - POP_SIMULATE_DIALOG_WIDTH)/2,(ScreenHeight - POP_SIMULATE_DIALOG_HEIGHT)/2,POP_SIMULATE_DIALOG_WIDTH,POP_SIMULATE_DIALOG_HEIGHT);

    chooseBar = new MyChooseBar(ui->widget_2);
    chooseBar->setParetWidget(this);

    QHBoxLayout * layout = new QHBoxLayout;
    layout->addWidget(chooseBar);
    layout->setContentsMargins(1,1,1,1);
    ui->widget_2->setLayout(layout);

    plainEdit = new MyPlainTextEdit(ui->widget_3);
    QHBoxLayout * layout1 = new QHBoxLayout;
    layout1->addWidget(plainEdit);
    layout1->setContentsMargins(1,1,1,1);
    ui->widget_3->setLayout(layout1);

    ui->switchExpress->setEnabled(false);

    connect(ui->quoteList,SIGNAL(itemPressed(QListWidgetItem*)),this,SLOT(addQuote(QListWidgetItem*)));
    connect(chooseBar,SIGNAL(confirmPressed()),this,SLOT(respConfirmPressed()));
}

//初始化控件
void MyConditionSetting::setJudgeProp(MyItem * item,bool isEditable)
{
    isEditState = isEditable;
    ui->quoteList->setEnabled(isEditable);
    plainEdit->setReadOnly(!isEditable);

    jprop = item->getJudegeProp();
    plainEdit->clear();
    plainEdit->setPlainText(jprop->express);

    QList<MyItem *> items = SimulateUtil::instance()->getCurrParentItem(item);
    ui->quoteList->clear();
    foreach(MyItem * tmpItem,items)
    {
        ServiceProperty * prop = tmpItem->getServiceProp();
        foreach(Parameter * para,prop->outputParas)
        {
            QString newItem = QString(COMBOX_START_FLAG)+":"+tmpItem->getText()+":"+para->pName+"]";
            ui->quoteList->addItem(newItem);
        }
    }

    ui->switchExpress->clear();
    ui->switchExpress->appendPlainText(jprop->switchExpress);
}

//添加引用类型
void MyConditionSetting::addQuote(QListWidgetItem* item)
{
    QTextCursor cursor = plainEdit->textCursor();

    cursor.beginEditBlock();
    cursor.insertText(item->text());
    cursor.endEditBlock();

    plainEdit->setTextCursor(cursor);
}

//目前暂未添加对表达式结果的验证
void MyConditionSetting::respConfirmPressed()
{
    if(!isEditState)
    {
        return;
    }
    jprop->express = plainEdit->toPlainText();
}

MyConditionSetting::~MyConditionSetting()
{
    if(plainEdit)
    {
        delete plainEdit;
        plainEdit = NULL;
    }

    if(chooseBar)
    {
        delete chooseBar;
        chooseBar = NULL;
    }

    delete ui;
}

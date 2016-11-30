#include "myoutputdialog.h"
#include "ui_myoutputdialog.h"

#include <QHBoxLayout>
#include <QTextCursor>
#include <QDebug>

#include "../global.h"
#include "../manager/listitemmanager.h"
#include "../simulate/simulateutil.h"
#include "../item/mygraphicsview.h"
#include "../item/myitem.h"
#include "mychoosebar.h"
#include "myplaintextedit.h"

MyOutputDialog::MyOutputDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MyOutputDialog)
{
    ui->setupUi(this);

    setWindowTitle("输入输出显示");

    setGeometry((ScreenWidth - POP_SIMULATE_DIALOG_WIDTH)/2,(ScreenHeight - POP_SIMULATE_DIALOG_HEIGHT)/2,POP_SIMULATE_DIALOG_WIDTH,POP_SIMULATE_DIALOG_HEIGHT);

    chooseBar = new MyChooseBar(ui->widget_2);
    chooseBar->setParetWidget(this);
    connect(chooseBar,SIGNAL(confirmPressed()),this,SLOT(respButtYesPress()));

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
}

//初始化控件设置
void MyOutputDialog::setOutput(MyItem *item, bool isEditable)
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

//确定
void MyOutputDialog::respButtYesPress()
{
    if(!isEditState)
    {
        return;
    }
    jprop->express = plainEdit->toPlainText();
}

//添加引用类型
void MyOutputDialog::addQuote(QListWidgetItem* item)
{
    QTextCursor cursor = plainEdit->textCursor();

    cursor.beginEditBlock();
    cursor.insertText(item->text());
    cursor.endEditBlock();

    plainEdit->setTextCursor(cursor);
}

MyOutputDialog::~MyOutputDialog()
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

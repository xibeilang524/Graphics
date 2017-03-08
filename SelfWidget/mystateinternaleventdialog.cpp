#include "mystateinternaleventdialog.h"
#include "ui_mystateinternaleventdialog.h"

#include "../global.h"
#include "mychoosebar.h"

#include <QDebug>

MyStateInternalEventDialog::MyStateInternalEventDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MyStateInternalEventDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("状态内部事件(守卫条件)设置");

    this->setGeometry((ScreenWidth-POP_SUB_DIALOG_WIDTH)/2,(ScreenHeight-POP_SUB_DIALOG_HEIGHT)/2
                      ,POP_SUB_DIALOG_WIDTH,POP_SUB_DIALOG_HEIGHT);

    isOkPressed = false;

    chooseBar = new MyChooseBar(ui->widget_3);
    chooseBar->setParetWidget(this);

    QHBoxLayout * layout = new QHBoxLayout;
    layout->addWidget(chooseBar);
    layout->setContentsMargins(1,1,1,1);
    ui->widget_3->setLayout(layout);

    connect(chooseBar,SIGNAL(confirmPressed()),this,SLOT(respConfirm()));
    connect(chooseBar,SIGNAL(cancelPressed()),this,SLOT(respCancel()));
}

//显示内部事件详细信息
void MyStateInternalEventDialog::showDesc(StatInnerProperty property)
{
    ui->eventName->setText(property.propName);
    ui->eventType->setCurrentIndex(property.propType.toInt());
    ui->eventContent->setPlainText(property.propDesc);
}

void MyStateInternalEventDialog::respConfirm()
{
    prop.propName = ui->eventName->text();
    prop.propType = QString::number(ui->eventType->currentIndex());
    prop.propDesc = ui->eventContent->toPlainText();
    isOkPressed = true;
}

void MyStateInternalEventDialog::respCancel()
{
    isOkPressed = false;
}

//获取返回值
bool MyStateInternalEventDialog::getResult(StatInnerProperty &property)
{
    if(isOkPressed)
    {
        property.propName = prop.propName;
        property.propType = prop.propType;
        property.propDesc = prop.propDesc;
    }
    return isOkPressed;
}

MyStateInternalEventDialog::~MyStateInternalEventDialog()
{
    delete chooseBar;
    delete ui;
}

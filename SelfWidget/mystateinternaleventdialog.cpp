#include "mystateinternaleventdialog.h"
#include "ui_mystateinternaleventdialog.h"

#include "../Header.h"
#include "../global.h"
#include "mychoosebar.h"

MyStateInternalEventDialog::MyStateInternalEventDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MyStateInternalEventDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("状态内部事件(守卫条件)设置");

    this->setGeometry((ScreenWidth-POP_SUB_DIALOG_WIDTH)/2,(ScreenHeight-POP_SUB_DIALOG_HEIGHT)/2
                      ,POP_SUB_DIALOG_WIDTH,POP_SUB_DIALOG_HEIGHT);

    chooseBar = new MyChooseBar(ui->widget_3);

    chooseBar->setParetWidget(this);

    QHBoxLayout * layout = new QHBoxLayout;
    layout->addWidget(chooseBar);
    layout->setContentsMargins(1,1,1,1);
    ui->widget_3->setLayout(layout);
}

MyStateInternalEventDialog::~MyStateInternalEventDialog()
{
    delete ui;
}

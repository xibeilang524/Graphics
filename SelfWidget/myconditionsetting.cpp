#include "myconditionsetting.h"
#include "ui_myconditionsetting.h"

#include <QHBoxLayout>

#include "../global.h"
#include "../Header.h"
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

    connect(ui->addQuote,SIGNAL(clicked()),this,SLOT(addQuote()));
}

//添加引用类型
void MyConditionSetting::addQuote()
{

}

MyConditionSetting::~MyConditionSetting()
{
    delete ui;
}

#include "hidesplit.h"
#include "ui_hidesplit.h"

#include <QVBoxLayout>

#include "../item/mygraphicsview.h"

#define SPLIT_FIX_WIDTH  10
#define SPLIT_FIX_HEIGHT 10

HideSplit::HideSplit(SpiltDirect direct, QWidget *parent) :
    splitDirect(direct),
    QWidget(parent),
    ui(new Ui::HideSplit)
{
    ui->setupUi(this);

    init();
}

void HideSplit::init()
{
    connect(ui->lbutt,SIGNAL(clicked()),this,SLOT(setContainerVisible()));
    connect(ui->rbutt,SIGNAL(clicked()),this,SLOT(setContainerVisible()));

    switch(splitDirect)
    {
        case SPLIT_LEFT:
                        {
                            QHBoxLayout * mainLayout = new QHBoxLayout;
                            mainLayout->setContentsMargins(0,0,0,0);

                            ui->vsplit->hide();
                            mainLayout->setSpacing(LAYOUT_SPACING);
                            mainLayout->addWidget(ui->hsplit);
                            mainLayout->addWidget(ui->container);

                            QVBoxLayout * layout = new QVBoxLayout;
                            layout->setContentsMargins(1,1,1,1);
                            ui->hsplit->setFixedWidth(SPLIT_FIX_WIDTH);

                            ui->container->setLayout(layout);
                            ui->widget->setLayout(mainLayout);
                        }

                         break;
        case SPLIT_TOP:
        break;
        case SPLIT_RIGHT:
                        {
                            QHBoxLayout * mainLayout = new QHBoxLayout;
                            mainLayout->setContentsMargins(0,0,0,0);

                            ui->vsplit->hide();
                            mainLayout->setSpacing(LAYOUT_SPACING);
                            mainLayout->addWidget(ui->container);
                            mainLayout->addWidget(ui->hsplit);

                            QVBoxLayout * layout = new QVBoxLayout;
                            layout->setContentsMargins(1,1,1,1);
                            ui->hsplit->setFixedWidth(SPLIT_FIX_HEIGHT);

                            ui->container->setLayout(layout);
                            ui->widget->setLayout(mainLayout);
                        }
                        break;
        case SPLIT_BOTTOM:
        break;
    }
}

//设置容器显示或者隐藏
void HideSplit::setContainerVisible()
{
    bool visibleFlag = ui->container->isVisible();

    if(visibleFlag)
    {
        visibleFlag = false;
        QWidget::setFixedWidth(SPLIT_FIX_WIDTH);
    }
    else
    {
        visibleFlag = true;
        this->setFixedWidth(fixWidth);
    }
    ui->container->setVisible(visibleFlag);
}

void HideSplit::setFixedWidth(int w)
{
    fixWidth = w;
    QWidget::setFixedWidth(fixWidth);
}

//返回容器窗口
QWidget * HideSplit::getContainer()
{
    return ui->container;
}

//增加
HideSplit * HideSplit::addWidget(SpiltDirect direct, QWidget *widget)
{
    HideSplit * split = new HideSplit(direct);

    widget->setParent(split->getContainer());
    split->addWidget(widget);

    return split;
}

void HideSplit::addWidget(QWidget *widget)
{
    QBoxLayout * boxLayout = qobject_cast<QBoxLayout * >(ui->container->layout());
    if(boxLayout)
    {
        boxLayout->addWidget(widget);
    }
}

HideSplit::~HideSplit()
{
   delete ui;
}

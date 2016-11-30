#include "myscrollarea.h"
#include "ui_myscrollarea.h"

#include <QDebug>
#include <QHBoxLayout>

#include "../Header.h"

MyScrollArea::MyScrollArea(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MyScrollArea)
{
    ui->setupUi(this);
    setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    widget = new QWidget;
//    widget->setStyleSheet("background-color:yellow");

    QHBoxLayout * mainLayout =  new QHBoxLayout;
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->addWidget(widget);

    itemContainer = new QWidget;
    itemContainer->setStyleSheet("background-color:red");
    itemContainer->setGeometry(0,0,1000,50);

    layout = new QHBoxLayout;
    layout->setContentsMargins(1,1,1,1);
    layout->addStretch(0);
    itemContainer->setLayout(layout);

    setLayout(mainLayout);
}

//根据窗口动态调整子窗口的位置
void MyScrollArea::resizeWidgetPos()
{
    int curWidth = this->width();
    int subWidth = itemContainer->width();
    int subHeight = itemContainer->height();

//    qDebug()<<subWidth<<"__"<<subHeight;
}

void MyScrollArea::insertWidget(int index, QWidget *item)
{
    layout->insertWidget(index-1,item);
    qDebug() <<__FUNCTION__
            <<index
            <<"\n";
    resizeWidgetPos();
}

void MyScrollArea::resizeEvent(QResizeEvent *)
{
    qDebug() <<__FUNCTION__
            <<this->width()<<"=="<<this->height()
            <<"\n";
    itemContainer->setGeometry(0,0,this->width(),this->height());
    //itemContainer->setGeometry(0,0,itemContainer->width(),itemContainer->height());
}

MyScrollArea::~MyScrollArea()
{
    delete ui;
}

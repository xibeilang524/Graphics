#include "mypageitem.h"

#include <QHBoxLayout>
#include <QEvent>
#include <QLabel>
#include <QPushButton>
#include <QDebug>

#include "../SelfWidget/mypageswitch.h"

MyPageItem::MyPageItem(QWidget *parent) :
    QWidget(parent)
{
    initWidget();
    setFixedWidth(90);

    setSelected(true);
}

void MyPageItem::initWidget()
{
    iconLabel = new QLabel(this);
    iconLabel->setObjectName("iconLabel");
    iconLabel->setFixedSize(15,15);
    iconLabel->setStyleSheet("border-image:url(:/images/pageFlag.png);");

    textLabel = new QLabel(this);
    textLabel->setObjectName("textLabel");
    textLabel->setAlignment(Qt::AlignCenter);
    textLabel->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    textLabel->setText("工作区");

    exitButt = new QPushButton(this);
    connect(exitButt,SIGNAL(clicked()),this,SLOT(deleteMe()));
    exitButt->setToolTip("关闭当前标签页");
    exitButt->setObjectName("exitButt");
    exitButt->setStyleSheet("QPushButton{border-image:url(:/images/pageClose.png);} \
                             QPushButton::hover{border-image:url(:/images/pageClose_hover.png)}");
    exitButt->setFixedSize(20,20);

    QHBoxLayout * layout = new QHBoxLayout;
    layout->setContentsMargins(1,1,1,1);
    layout->setSpacing(2);
    layout->addWidget(iconLabel);
    layout->addWidget(textLabel);
    layout->addWidget(exitButt);

    setLayout(layout);
}

//删除子页面
void MyPageItem::deleteMe()
{
    emit deletePage(id);
}

//设置是否选中
void MyPageItem::setSelected(bool isSelected)
{
    isSelected = isSelected;
    if(isSelected)
    {
        textLabel->setStyleSheet("background-color:rgba(255,0,0,190);color:white;border-radius:3px");
    }
    else
    {
        textLabel->setStyleSheet("background-color:rgba(255,0,0,0);color:black");
    }
}

//设置显示的字符
void MyPageItem::setText(QString text)
{
    textLabel->setText(text);
}

void MyPageItem::mousePressEvent(QMouseEvent *)
{
    emit switchPage(id);
}


PageManager * PageManager::manager = NULL;

PageManager::PageManager()
{

}

PageManager * PageManager::instance()
{
    if(!manager)
    {
        manager = new PageManager;
    }
    return manager;
}

MyPageItem * PageManager::addPageItem()
{
    MyPageItem * item = new MyPageItem(MyPageSwitch::instance());
    return item;
}


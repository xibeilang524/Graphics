#include "lefticonwidget.h"

#include <QToolBox>
#include <QHBoxLayout>
#include <QDebug>

#include "listitemmanager.h"
#include "mylistwidget.h"
#include "listitemmanager.h"
#include "../Constants.h"

using namespace Graphics;

LeftIconWidget::LeftIconWidget(QWidget *parent):
    QWidget(parent)
{
    setFixedWidth(150);

    initWidget();
}

//初始化控件
void LeftIconWidget::initWidget()
{
    QHBoxLayout * mainLayout = new QHBoxLayout;
    mainLayout->setContentsMargins(1,1,1,1);

    toolBox = new QToolBox(this);

    polygonWidget = new MyListWidget;

    nodeWidget = new MyListWidget;

    initListItems();

    toolBox->addItem(polygonWidget,"Polygon");
    toolBox->addItem(nodeWidget,"Nodes");

    mainLayout->addWidget(toolBox);

    this->setLayout(mainLayout);
}

//初始化列表item
void LeftIconWidget::initListItems()
{
    //插入多边形
    MyListItem * square = ListItemManager::instance()->createListItem(Constants::SQUARE_ID,QIcon(":/images/square.png"),"正方形",polygonWidget);
    square->setData(Qt::UserRole,(int)GRA_SQUARE);
    polygonWidget->addItem(square);

    MyListItem * rect = ListItemManager::instance()->createListItem(Constants::RECT_ID,QIcon(":/images/rectange.png"),"矩形",polygonWidget);
    rect->setData(Qt::UserRole,(int)GRA_RECT);
    polygonWidget->addItem(rect);

    MyListItem * roundRect = ListItemManager::instance()->createListItem(Constants::ROUNDRECT_ID,QIcon(":/images/roundedrect.png"),"圆角矩形",polygonWidget);
    roundRect->setData(Qt::UserRole,(int)GRA_ROUND_RECT);
    polygonWidget->addItem(roundRect);

    MyListItem * circle = ListItemManager::instance()->createListItem(Constants::CIRCLE_ID,QIcon(":/images/circle.png"),"圆",polygonWidget);
    circle->setData(Qt::UserRole,(int)GRA_CIRCLE);
    polygonWidget->addItem(circle);

    MyListItem * ellipse = ListItemManager::instance()->createListItem(Constants::ELLIPSE_ID,QIcon(":/images/ellipse.png"),"椭圆",polygonWidget);
    ellipse->setData(Qt::UserRole,(int)GRA_ELLIPSE);
    polygonWidget->addItem(ellipse);

    MyListItem * polygon = ListItemManager::instance()->createListItem(Constants::POLYGON_ID,QIcon(":/images/diamonds.png"),"菱形",polygonWidget);
    polygon->setData(Qt::UserRole,(int)GRA_POLYGON);
    polygonWidget->addItem(polygon);

    MyListItem * text = ListItemManager::instance()->createListItem(Constants::TEXT_ID,QIcon(":/images/text.png"),"文字",polygonWidget);
    text->setData(Qt::UserRole,(int)GRA_TEXT);
    polygonWidget->addItem(text);

    //插入节点
    MyListItem * nodePort = ListItemManager::instance()->createListItem(Constants::NODE_PORT_ID,QIcon(":/images/nodePort.png"),"端口",nodeWidget);
    nodePort->setData(Qt::UserRole,(int)GRA_NODE_PORT);
    nodeWidget->addItem(nodePort);
}

LeftIconWidget::~LeftIconWidget()
{
    if(polygonWidget)
    {
        delete polygonWidget;
        polygonWidget = NULL;
    }

    if(nodeWidget)
    {
        delete nodeWidget;
        nodeWidget = NULL;
    }

    if(toolBox)
    {
        delete toolBox;
        toolBox = NULL;
    }
}

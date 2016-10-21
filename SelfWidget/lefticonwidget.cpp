#include "lefticonwidget.h"

#include <QToolBox>
#include <QHBoxLayout>

#include "../manager/listitemmanager.h"
#include "mylistwidget.h"
#include "../Constants.h"
#include "../Header.h"

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

    toolBox->addItem(polygonWidget,"流程图");
    toolBox->addItem(nodeWidget,"Nodes");

    mainLayout->addWidget(toolBox);

    this->setLayout(mainLayout);
}

//初始化列表item
void LeftIconWidget::initListItems()
{
    //插入多边形
    MyListItem * roundRect = ListItemManager::instance()->createListItem(Constants::ROUNDRECT_ID,QIcon(":/images/roundedrect.png"),"开始/结束",polygonWidget);
    roundRect->setData(Qt::UserRole,(int)GRA_ROUND_RECT);
    polygonWidget->addItem(roundRect);

    MyListItem * rect = ListItemManager::instance()->createListItem(Constants::RECT_ID,QIcon(":/images/rectange.png"),"流程",polygonWidget);
    rect->setData(Qt::UserRole,(int)GRA_RECT);
    polygonWidget->addItem(rect);

    MyListItem * polygon = ListItemManager::instance()->createListItem(Constants::POLYGON_ID,QIcon(":/images/diamonds.png"),"判定",polygonWidget);
    polygon->setData(Qt::UserRole,(int)GRA_POLYGON);
    polygonWidget->addItem(polygon);

    MyListItem * paralleogram = ListItemManager::instance()->createListItem(Constants::PARALLEOGRAM_ID,QIcon(":/images/parallelogram.png"),"输入输出",polygonWidget);
    paralleogram->setData(Qt::UserRole,(int)GRA_PARALLELOGRAM);
    polygonWidget->addItem(paralleogram);

    MyListItem * loopUp = ListItemManager::instance()->createListItem(Constants::LOOP_UP_ID,QIcon(":/images/loop_up.png"),"循环上限",polygonWidget);
    loopUp->setData(Qt::UserRole,(int)GRA_LOOP_UP);
    polygonWidget->addItem(loopUp);

    MyListItem * loopDown = ListItemManager::instance()->createListItem(Constants::LOOP_DOWN_ID,QIcon(":/images/loop_down.png"),"循环下限",polygonWidget);
    loopDown->setData(Qt::UserRole,(int)GRA_LOOP_DOWN);
    polygonWidget->addItem(loopDown);

    MyListItem * parallel = ListItemManager::instance()->createListItem(Constants::PARALLEL_ID,QIcon(":/images/parallel.png"),"并  行",polygonWidget);
    parallel->setData(Qt::UserRole,(int)GAR_PARALLE);
    polygonWidget->addItem(parallel);

    MyListItem * circle = ListItemManager::instance()->createListItem(Constants::CIRCLE_ID,QIcon(":/images/circle.png"),"页面内引用",polygonWidget);
    circle->setData(Qt::UserRole,(int)GRA_CIRCLE);
    polygonWidget->addItem(circle);

    MyListItem * annotation = ListItemManager::instance()->createListItem(Constants::ANNOTATION_ID,QIcon(":/images/annotation.png"),"注  解",polygonWidget);
    annotation->setData(Qt::UserRole,(int)GRA_ANNOTATION);
    polygonWidget->addItem(annotation);

//    MyListItem * square = ListItemManager::instance()->createListItem(Constants::SQUARE_ID,QIcon(":/images/square.png"),"正方形",polygonWidget);
//    square->setData(Qt::UserRole,(int)GRA_SQUARE);
//    polygonWidget->addItem(square);

//    MyListItem * ellipse = ListItemManager::instance()->createListItem(Constants::ELLIPSE_ID,QIcon(":/images/ellipse.png"),"椭圆",polygonWidget);
//    ellipse->setData(Qt::UserRole,(int)GRA_ELLIPSE);
//    polygonWidget->addItem(ellipse);

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

#include "lefticonwidget.h"

#include <QToolBox>
#include <QHBoxLayout>
#include <QDebug>

#include "listitemmanager.h"
#include "mylistwidget.h"

LeftIconWidget::LeftIconWidget(QWidget *parent):
    QWidget(parent)
{
    setFixedWidth(120);


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
    QListWidgetItem * square = new QListWidgetItem(QIcon(":/images/square.png"),"正方形",polygonWidget);
    square->setSizeHint(QSize(60,60));

    square->setData(Qt::UserRole,"MyListItem");
    polygonWidget->addItem(square);

    polygonWidget->addItem(new QListWidgetItem(QIcon(":/images/rectange.png"),"矩形",polygonWidget));
    polygonWidget->addItem(new QListWidgetItem(QIcon(":/images/roundedrect.png"),"圆角矩形",polygonWidget));
    polygonWidget->addItem(new QListWidgetItem(QIcon(":/images/circle.png"),"圆",polygonWidget));
    polygonWidget->addItem(new QListWidgetItem(QIcon(":/images/ellipse.png"),"椭圆",polygonWidget));
    polygonWidget->addItem(new QListWidgetItem(QIcon(":/images/diamonds.png"),"菱形",polygonWidget));

    //插入节点
    nodeWidget->addItem(new QListWidgetItem(QIcon(":/images/linepointer.png"),"线条",nodeWidget));
    nodeWidget->addItem(new QListWidgetItem(QIcon(":/images/vectorLine.png"),"连接线",nodeWidget));
}

LeftIconWidget::~LeftIconWidget()
{
    delete toolBox;
    delete polygonWidget;
    delete nodeWidget;
}

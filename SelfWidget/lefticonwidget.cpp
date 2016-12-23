#include "lefticonwidget.h"

#include <QToolBox>
#include <QHBoxLayout>
#include <QKeyEvent>

#include "../manager/listitemmanager.h"
#include "mylistwidget.h"
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
#ifdef ADD_STATE_MODEL
    stateWidget = new MyListWidget;
    maskWidget = new MyListWidget;
#endif
    initListItems();

    toolBox->addItem(polygonWidget,"流程图");
#ifdef ADD_STATE_MODEL
    toolBox->addItem(stateWidget,"状态图");
#endif
    toolBox->addItem(nodeWidget,"原子图");
#ifdef ADD_STATE_MODEL
    toolBox->addItem(maskWidget,"遮罩");
#endif

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
#ifdef ADD_STATE_MODEL
    //状态机
    MyListItem * startState = ListItemManager::instance()->createListItem(Constants::STATE_START_ID,QIcon(":/images/State_Start.png"),"开始",stateWidget);
    startState->setData(Qt::UserRole,(int)GRA_STATE_START);
    stateWidget->addItem(startState);

    MyListItem * endState = ListItemManager::instance()->createListItem(Constants::STATE_END_ID,QIcon(":/images/State_End.png"),"结束",stateWidget);
    endState->setData(Qt::UserRole,(int)GRA_STATE_END);
    stateWidget->addItem(endState);

    MyListItem * processState = ListItemManager::instance()->createListItem(Constants::STATE_PROCESS_ID,QIcon(":/images/roundedrect.png"),"处理",stateWidget);
    processState->setData(Qt::UserRole,(int)GRA_STATE_PROCESS);
    stateWidget->addItem(processState);

    //遮罩maskWidget
    MyListItem * rectMask = ListItemManager::instance()->createListItem(Constants::MASK_RECT_ID,QIcon(":/images/rectange.png"),"矩形遮罩",maskWidget);
    rectMask->setData(Qt::UserRole,(int)GRA_MASK_RECT);
    maskWidget->addItem(rectMask);

    MyListItem * boundRectMask = ListItemManager::instance()->createListItem(Constants::MASK_BOUND_RECT_ID,QIcon(":/images/roundedrect.png"),"圆角遮罩",maskWidget);
    boundRectMask->setData(Qt::UserRole,(int)GRA_MASK_BOUND_RECT);
    maskWidget->addItem(boundRectMask);

    MyListItem * circleMask = ListItemManager::instance()->createListItem(Constants::MASK_CIRCLE_ID,QIcon(":/images/circle.png"),"圆遮罩",maskWidget);
    circleMask->setData(Qt::UserRole,(int)GRA_MASK_CIRCLE);
    maskWidget->addItem(circleMask);

    //插入节点
    MyListItem * atomProcessPort = ListItemManager::instance()->createListItem(Constants::NODE_PROCESS_ID,QIcon(":/images/rectange.png"),"原子处理",nodeWidget);
    atomProcessPort->setData(Qt::UserRole,(int)GRA_NODE_PROCESS);
    nodeWidget->addItem(atomProcessPort);
#endif

    MyListItem * nodePort = ListItemManager::instance()->createListItem(Constants::NODE_PORT_ID,QIcon(":/images/nodePort.png"),"矩形端口",nodeWidget);
    nodePort->setData(Qt::UserRole,(int)GRA_NODE_PORT);
    nodeWidget->addItem(nodePort);

    MyListItem * nodeTriangleInPort = ListItemManager::instance()->createListItem(Constants::NODE_TRIANGLE_IN_ID,QIcon(":/images/nodeTriangle.png"),"三角输出端口",nodeWidget);
    nodeTriangleInPort->setData(Qt::UserRole,(int)GRA_NODE_TRIANGLE_OUT);
    nodeWidget->addItem(nodeTriangleInPort);

    MyListItem * nodeTriangleOutPort = ListItemManager::instance()->createListItem(Constants::NODE_TRIANGLE_OUT_ID,QIcon(":/images/nodeTriangle.png"),"三角输入端口",nodeWidget);
    nodeTriangleOutPort->setData(Qt::UserRole,(int)GRA_NODE_TRIANGLE_IN);
    nodeWidget->addItem(nodeTriangleOutPort);

    MyListItem * nodeHalfCircleIn = ListItemManager::instance()->createListItem(Constants::NODE_HALF_CIRCLE_IN_ID,QIcon(":/images/nodeHalfCircleIn.png"),"D形输入端口",nodeWidget);
    nodeHalfCircleIn->setData(Qt::UserRole,(int)GRA_NODE_HALF_CIRCLE_IN);
    nodeWidget->addItem(nodeHalfCircleIn);

    MyListItem * nodeHalfCircleOut = ListItemManager::instance()->createListItem(Constants::NODE_HALF_CIRCLE_OUT_ID,QIcon(":/images/nodeHalfCircleOut.png"),"D形输出端口",nodeWidget);
    nodeHalfCircleOut->setData(Qt::UserRole,(int)GRA_NODE_HALF_CIRCLE_OUT);
    nodeWidget->addItem(nodeHalfCircleOut);

    MyListItem * nodeCircle = ListItemManager::instance()->createListItem(Constants::NODE_CIRCLE_ID,QIcon(":/images/circle.png"),"圆形端口",nodeWidget);
    nodeCircle->setData(Qt::UserRole,(int)GRA_NODE_CIRCLE);
    nodeWidget->addItem(nodeCircle);
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
#ifdef ADD_STATE_MODEL
    if(stateWidget)
    {
        delete stateWidget;
        stateWidget = NULL;
    }

    if(maskWidget)
    {
        delete maskWidget;
        maskWidget = NULL;
    }
#endif
    if(toolBox)
    {
        delete toolBox;
        toolBox = NULL;
    }
}

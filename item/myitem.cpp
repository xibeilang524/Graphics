#include "myitem.h"

#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneHoverEvent>
#include <QGraphicsSceneContextMenuEvent>
#include <QCursor>
#include <QPainter>
#include <QMenu>
#include <QDebug>
#include <QGraphicsScene>
#include <QPointF>
#include <QGraphicsSceneDragDropEvent>
#include <QMimeData>
#include <QUrl>
#include <QKeyEvent>

#include "myarrow.h"
#include "mytextitem.h"
#include "mynodeport.h"
#include "mygraphicsview.h"
#include "dragpoint.h"
#include "rotateline.h"
#include "draglinepoint.h"
#include "../global.h"
#include "../util.h"
#include "../manager/menumanager.h"

#include "qmath.h"

#define QCOS(a) qCos(a*M_PI/180)
#define QSIN(a) qSin(a*M_PI/180)

#define CROSS_RADIUS 8               //绘制拖入十字星半径
#define POINT_FIVE   0.5             //控件竖直和水平的长度比例，用于绘制矩形等非等边图形
#define POINT_TWO_FIVE 0.25
#define POINT_ONE_EIGHTH 0.125
#define ANNOTATION_SHORT_LINE  7     //注解短边的长度
#define MAX_LOOP_RADIUS    25        //注解短边最大长度
#define STATE_END_RADIUS   3         //状态机结束圆与Circle半径差

QDataStream & operator <<(QDataStream & stream,JudgeProperty * prop)
{
    stream<<prop->express<<prop->switchExpress;
    return stream;
}

QDataStream & operator >>(QDataStream & stream,JudgeProperty * prop)
{
    stream>>prop->express>>prop->switchExpress;
    return stream;
}

//对服务的参数进行重载
QDataStream & operator <<(QDataStream & stream,Parameter * para)
{
    stream<<para->pId<<para->pName<<para->pType<<para->pValue<<para->pRemark;
    return stream;
}

QDataStream & operator >>(QDataStream & stream,Parameter * para)
{
    stream>>para->pId>>para->pName>>para->pType>>para->pValue>>para->pRemark;
    return stream;
}

//对ServiceProperty进行重载
QDataStream & operator <<(QDataStream & stream,ServiceProperty * prop)
{
    int inputSize = prop->inputParas.size();
    int outputSize = prop->outputParas.size();

    stream<<prop->hasSettInfo<<prop->id<<prop->serviceName<<prop->status
            <<prop->descirption<<prop->servicePath<<prop->method<<inputSize
              <<outputSize<<prop->isAsNextInput;
    //分别对输入和输出的参数进行保存
    foreach(Parameter * para,prop->inputParas)
    {
        stream<<para;
    }

    foreach(Parameter * para,prop->outputParas)
    {
        stream<<para;
    }
    return stream;
}

QDataStream & operator >>(QDataStream & stream,ServiceProperty * prop)
{
    int inputSize = 0;
    int outputSize = 0;

    stream>>prop->hasSettInfo>>prop->id>>prop->serviceName>>prop->status
            >>prop->descirption>>prop->servicePath>>prop->method>>inputSize
              >>outputSize>>prop->isAsNextInput;

    //分别对输入和输出的参数进行保存
    for(int i = 0; i < inputSize ; i++)
    {
        Parameter * para = new Parameter;
        stream>>para;
        prop->inputParas.append(para);
    }

    for(int i = 0; i < outputSize ; i++)
    {
        Parameter * para = new Parameter;
        stream>>para;
        prop->outputParas.append(para);
    }
    return stream;
}

//对循环条件进行重载
QDataStream & operator <<(QDataStream &stream,LoopProperty * loopProp)
{
    stream<<loopProp->signalList.size()<<loopProp->varList.size()
            <<loopProp->expList.size()<<loopProp->fexpList.size();

    foreach(SignalVari * vari,loopProp->signalList)
    {
        stream<<vari;
    }

    foreach(VariableDefine * vari,loopProp->varList)
    {
        stream<<vari;
    }

    foreach(ExpressDefine * vari,loopProp->expList)
    {
        stream<<vari;
    }

    foreach(FinalExpressDefine * vari,loopProp->fexpList)
    {
        stream<<vari;
    }
    return stream;
}

//对循环条件进行重载
QDataStream & operator >>(QDataStream &stream,LoopProperty * loopProp)
{
    int slist = 0;
    int vlist = 0;
    int elist = 0;
    int felist = 0;
    stream>>slist>>vlist>>elist>>felist;

    for(int i = 0; i < slist; i++)
    {
        SignalVari * vari = new SignalVari;
        stream>>vari;

        loopProp->signalList.append(vari);
    }

    for(int i = 0; i < vlist; i++)
    {
        VariableDefine * vari = new VariableDefine;
        stream>>vari;
        loopProp->varList.append(vari);
    }

    for(int i = 0; i < elist; i++)
    {
        ExpressDefine * vari = new ExpressDefine;
        stream>>vari;
        loopProp->expList.append(vari);
    }

    for(int i = 0; i < felist; i++)
    {
        FinalExpressDefine * vari = new FinalExpressDefine;
        stream>>vari;
        loopProp->fexpList.append(vari);
    }

    return stream;
}

//VariableDefine
QDataStream & operator <<(QDataStream & stream ,VariableDefine * vari)
{
    stream<<vari->type<<vari->name<<vari->value<<vari->pRemark;
    return stream;
}

QDataStream & operator >>(QDataStream & stream ,VariableDefine * vari)
{
    stream>>vari->type>>vari->name>>vari->value>>vari->pRemark;
    return stream;
}

//ExpressDefine
QDataStream & operator <<(QDataStream & stream ,ExpressDefine * vari)
{
    stream<<vari->name<<vari->expressType<<vari->value<<vari->pRemark;
    return stream;
}

QDataStream & operator >>(QDataStream & stream ,ExpressDefine * vari)
{
    stream>>vari->name>>vari->expressType>>vari->value>>vari->pRemark;
    return stream;
}

//FinalExpressDefine
QDataStream & operator <<(QDataStream & stream ,FinalExpressDefine * vari)
{
    stream<<vari->name<<vari->expressType<<vari->value<<vari->pRemark;
    return stream;
}

QDataStream & operator >>(QDataStream & stream ,FinalExpressDefine * vari)
{
    stream>>vari->name>>vari->expressType>>vari->value>>vari->pRemark;
    return stream;
}

//SignalVari
QDataStream & operator <<(QDataStream & stream ,SignalVari * vari)
{
    stream<<vari->variName<<vari->isLegal<<vari->initialValue<<vari->finalValue
            <<vari->middlValue<<vari->operateSymbol<<vari->selfOperateSymbol<<vari->selfOperateValue;
    return stream;
}

QDataStream & operator >>(QDataStream & stream ,SignalVari * vari)
{
    stream>>vari->variName>>vari->isLegal>>vari->initialValue>>vari->finalValue
            >>vari->middlValue>>vari->operateSymbol>>vari->selfOperateSymbol>>vari->selfOperateValue;
    return stream;
}

//对MyRect的输出进行重载
QDataStream & operator <<(QDataStream &stream,MyRect & rect)
{
    stream<<rect.x<<rect.y<<rect.width<<rect.height;
    return stream;
}

QDataStream & operator >>(QDataStream &stream,MyRect & rect)
{
    stream>>rect.x>>rect.y>>rect.width>>rect.height;
    return stream;
}

QDataStream & operator <<(QDataStream &stream,LineType & type)
{
    int tmp = (int)type;
    stream<<tmp;
    return stream;
}

QDataStream & operator >>(QDataStream &stream,LineType & type)
{
    int tmp;
    stream>>tmp;
    type = (LineType)tmp;
    return stream;
}

QDataStream & operator <<(QDataStream &stream,PointType & type)
{
    int tmp = (int)type;
    stream<<tmp;
    return stream;
}

QDataStream & operator >>(QDataStream &stream,PointType & type)
{
    int tmp;
    stream>>tmp;
    type = (PointType)tmp;
    return stream;
}

QDataStream & operator <<(QDataStream &stream,AddLineType & type)
{
    int tmp = (int)type;
    stream<<tmp;
    return stream;
}

QDataStream & operator >>(QDataStream &stream,AddLineType & type)
{
    int tmp;
    stream>>tmp;
    type = (AddLineType)tmp;
    return stream;
}

//写入属性
QDataStream & operator <<(QDataStream & stream,ItemProperty & prop)
{
    stream<<prop.startItemID<<prop.endItemID<<prop.associativeID
           <<prop.isNeedBrush<<prop.itemBrush
            <<prop.isNeedBorder<<prop.itemPen<<prop.itemRect
             <<prop.alphaValue<<prop.rotateDegree
              <<prop.isFont<<prop.content<<prop.itemFont<<prop.fontColor
               <<prop.zValue<<prop.lineType<<prop.startLineType<<prop.endLineType<<prop.startPointType<<prop.endPointType
                <<prop.isMoveable<<prop.ptype;
    return stream;
}

//读属性
QDataStream & operator >>(QDataStream & stream,ItemProperty & prop)
{
    int ptype = 0;
    stream>>prop.startItemID>>prop.endItemID>>prop.associativeID
           >>prop.isNeedBrush>>prop.itemBrush
            >>prop.isNeedBorder>>prop.itemPen>>prop.itemRect
             >>prop.alphaValue>>prop.rotateDegree
              >>prop.isFont>>prop.content>>prop.itemFont>>prop.fontColor
               >>prop.zValue>>prop.lineType>>prop.startLineType>>prop.endLineType>>prop.startPointType>>prop.endPointType
                >>prop.isMoveable>>ptype;
    prop.ptype = (ProcessType)ptype;

    return stream;
}

QDataStream & operator <<(QDataStream &stream,MyItem * item)
{
    int type = item->currItemType;

    stream<<type<<item->property;
    if(item->getProperty().ptype == PRO_PROCESS)
    {
        stream<<item->getServiceProp();
    }
    else if(item->getProperty().ptype == PRO_JUDGE || item->getProperty().ptype == PRO_INPUT)
    {
        stream<<item->getJudegeProp();
    }
    else if(item->getProperty().ptype == PRO_LOOP)
    {
        stream<<item->getLoopProp();
    }

    return stream;
}

QDataStream & operator >>(QDataStream &stream,MyItem * item)
{
    int type;
    ItemProperty prop;

    stream>>type>>prop;
    item->currItemType = (GraphicsType)type;

    item->setProperty(prop);

    if(prop.ptype == PRO_PROCESS)
    {
        ServiceProperty * sprop = new ServiceProperty;

        stream>>sprop;
        item->setServiceProperty(sprop);

        delete sprop;
    }
    else if(prop.ptype == PRO_LOOP)
    {
        LoopProperty * lprop = new LoopProperty;
        stream>>lprop;
        item->setLoopProp(lprop);

        delete lprop;
    }
    else if(prop.ptype == PRO_JUDGE || prop.ptype == PRO_INPUT)
    {
        JudgeProperty * jprop = new JudgeProperty;
        stream>>jprop;
        item->setJudgeProp(jprop);

        delete jprop;
    }

    return stream;
}

MyItem::MyItem(GraphicsType itemType, QGraphicsScene *parentScene, QObject *parent1, QGraphicsItem *parent2):
    parentScene(parentScene),
    MySuperItem(itemType,parent2,parent1)
{
    radius = 60;
    isDragging = false;            //默认无拖入
    isPrepareLine = false;
    isSimulateHigh = false;
    lightLevel = LEVEL_NORMAL;

    setFlag(QGraphicsItem::ItemIsMovable,true);
    setFlag(QGraphicsItem::ItemIsSelectable,true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges,true);
    setAcceptDrops(true);
    setAcceptHoverEvents(true);

    prepareGeometryChange();

    switch(currItemType)
    {
        case GRA_SQUARE:
                           boundRect = QRectF(-radius,-radius,2*radius,2*radius);   //设置范围时同时也默认指定了其中心点坐标(0,0)
                           break;
        case GRA_RECT:
        case GRA_ANNOTATION:
        case GRA_ROUND_RECT:
        case GRA_ELLIPSE:
        case GRA_POLYGON:
        case GRA_PARALLELOGRAM:
        case GRA_LOOP_UP:
        case GRA_LOOP_DOWN:
#ifdef ADD_STATE_MODEL
        case GRA_STATE_PROCESS:
#endif
                             boundRect = QRectF(-radius,-POINT_FIVE*radius,2*radius,radius);
                             break;
        case GRA_CIRCLE:
#ifdef ADD_STATE_MODEL
        case GRA_STATE_START:
        case GRA_STATE_END:
#endif
                             boundRect = QRectF(-POINT_FIVE*radius,-POINT_FIVE*radius,radius,radius);
                             break;
        case GAR_PARALLE:
                             boundRect = QRectF(-radius,-POINT_ONE_EIGHTH*radius,2*radius,POINT_TWO_FIVE*radius);
                             break;
#ifdef ADD_STATE_MODEL
        case GRA_MASK_RECT:
        case GRA_MASK_BOUND_RECT:
                             radius*=2;
                             boundRect = QRectF(-POINT_FIVE*radius,-radius,radius,2*radius);
                             break;
        case GRA_MASK_CIRCLE:
                             boundRect = QRectF(-radius,-radius,2*radius,2*radius);
                             break;
        case GRA_NODE_PROCESS:
                            radius*=2;
                            boundRect = QRectF(-radius,-POINT_FIVE*radius,2*radius,radius);
                            break;
#endif
        default:
             break;
    }
    //【!!初始化不同类型图形】
    setInitalPolygon(boundRect,boundRect.width()/2,boundRect.height()/2,boundRect.width(),boundRect.height());

    property.isMoveable = true;         //默认可以移动
#ifdef ADD_STATE_MODEL
    startProp.content = property.content;
    stateModeProp.stateName = property.content;

    if(currItemType == GRA_STATE_START||currItemType == GRA_STATE_END)
    {
        property.itemBrush = QBrush(Qt::black);
    }
    //遮罩
    else if(currItemType == GRA_MASK_RECT || currItemType == GRA_MASK_BOUND_RECT ||currItemType == GRA_MASK_CIRCLE)
    {
        property.itemBrush = QBrush(Qt::blue,Qt::BDiagPattern);
    }
    else if(currItemType == GRA_NODE_PROCESS)
    {
        property.itemBrush = QBrush(QColor(226,226,226));
    }
    else
    {
#endif
        property.itemBrush = QBrush(GLOBAL_ITEM_BRUSH);
#ifdef ADD_STATE_MODEL
    }
#endif

    property.itemPen = QPen(Qt::black,2,Qt::SolidLine);

    property.itemRect.width = boundRect.width();
    property.itemRect.height = boundRect.height();

    property.itemFont = QFont("黑体",FONT_DEAFULT_PIX);

    setBrush(property.itemBrush);

    serviceProp = new ServiceProperty;
    loopProperty = new LoopProperty;
    judgeProperty = new JudgeProperty;

    currMouseType = MOUSE_NONE;
    isNeedBorder = false;

    selectedPenWidth = 1;
    selectedPen.setColor(Qt::black);
    selectedPen.setStyle(Qt::DashLine);
    selectedPen.setWidth(selectedPenWidth);

    initComponentItem();
}

//初始化子控件
void MyItem::initComponentItem()
{
    //旋转线条
    rotateLine = new RotateLine(GRA_ROTATE_POINT,this);
    connect(rotateLine,SIGNAL(rotateItem(MouseType,qreal)),this,SLOT(procRotate(MouseType,qreal)));
    updateRotateLinePos();

    //文字信息
    myTextItem = new MyTextItem(GRA_TEXT,this);
    myTextItem->setTextExistType(TEXT_CHILD);
    connect(myTextItem,SIGNAL(updateTextGeometry()),this,SLOT(procUpdateTextGeometry()));

    myTextItem->setTextInteractionFlags(Qt::NoTextInteraction);
    myTextItem->setFlag(QGraphicsItem::ItemIsMovable,false);
    myTextItem->setFlag(QGraphicsItem::ItemIsSelectable,false);
    myTextItem->cleartText();

    //四个顶点的拖拽点
    leftTopPoint = new DragPoint(TOP_LEFT,GRA_DRAG_POINT,this);
    rightTopPoint = new DragPoint(TOP_RIGHT,GRA_DRAG_POINT,this);
    leftBottomPoint = new DragPoint(BOTTOM_LEFT,GRA_DRAG_POINT,this);
    rightBottomPoint = new DragPoint(BOTTOM_RIGHT,GRA_DRAG_POINT,this);

    //四边中点的直线绘制点
    topLinePoint = new DragLinePoint(TOP_MIDDLE,GRA_DRAG_LING,this);
    leftLinePoint = new DragLinePoint(MIDDLE_LEFT,GRA_DRAG_LING,this);
    rightLinePoint = new DragLinePoint(MIDDLE_RIGHT,GRA_DRAG_LING,this);
    bottomLinePoint = new DragLinePoint(BOTTOM_MIDDLE,GRA_DRAG_LING,this);

    setDragPointVisible(false);
    procResizeItem();
}

void MyItem::setPos(const QPointF &pos)
{
    property.itemRect.x = pos.x();
    property.itemRect.y = pos.y();

    QGraphicsPolygonItem::setPos(pos);
}

void MyItem::setZValue(qreal z)
{
    property.zValue = z;
    QGraphicsPolygonItem::setZValue(z);
}

QString MyItem::getText()
{
    return myTextItem->toPlainText();
}

//更新文字信息，同时更新textItem在item中的位置
void MyItem::setText(QString text)
{
    property.content = text;
    myTextItem->setPlainText(text);

    procUpdateTextGeometry();
}

//动态调整输入字符框的位置尺寸
void MyItem::procUpdateTextGeometry()
{
    QRectF rectF = myTextItem->getBoundRect();
    myTextItem->setPos(-rectF.width()/2,-rectF.height()/2);
}

void MyItem::updateRotateLinePos()
{
    int posX = boundingRect().center().x();
    int posY = boundingRect().topLeft().y() - 5*rotateLine->boundingRect().height();

    rotateLine->setPos(posX,posY);
}

QRectF MyItem::boundingRect()const
{
    return boundRect;
}

void MyItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    painter->save();

    if(property.isNeedBorder)
    {
        painter->setPen(property.itemPen);
    }
    else
    {
        painter->setPen(Qt::NoPen);
    }

    if(property.isNeedBrush)
    {
        painter->setBrush(property.itemBrush);
    }

    if(currItemType == GRA_ANNOTATION)
    {
        painter->drawLine(boundRect.topLeft(),QPointF(boundRect.topLeft().x()+ANNOTATION_SHORT_LINE,boundRect.topLeft().y()));
        painter->drawLine(boundRect.topLeft(),boundRect.bottomLeft());
        painter->drawLine(boundRect.bottomLeft(),QPointF(boundRect.bottomLeft().x()+ANNOTATION_SHORT_LINE,boundRect.bottomLeft().y()));
    }
    else if(currItemType == GAR_PARALLE)
    {
        painter->drawLine(boundRect.topLeft(),boundRect.topRight());
        painter->drawLine(boundRect.bottomLeft(),boundRect.bottomRight());
    }
    else
    {
        if(isSimulateHigh)
        {
            if(lightLevel == LEVEL_MIDDLE)
            {
                painter->setPen(QPen(Qt::red,3));
            }
            else if(lightLevel == LEVEL_HIGH)
            {
                painter->setPen(QPen(Qt::blue,3));
            }
        }
#ifdef ADD_STATE_MODEL
        if(currItemType == GRA_STATE_END)
        {
            painter->save();
            painter->setBrush(Qt::NoBrush);
            painter->setPen(QPen(Qt::black,2));
            painter->drawEllipse(boundRect);
            painter->restore();
        }
#endif
        painter->drawPolygon(itemPolygon);
    }

    //线条绘制状态下，鼠标进入控件，进行样式的改变。
//    if(isPrepareLine)
//    {
//        painter->save();
//        painter->setPen(QPen(Qt::green,2));
//        painter->drawPolygon(itemPolygon);

//        painter->setPen(QPen(Qt::gray,5));
//        painter->drawPoint(boundRect.topLeft().x(),0);
//        painter->drawPoint(boundRect.topRight().x(),0);
//        painter->drawPoint(0,boundRect.topLeft().y());
//        painter->drawPoint(0,boundRect.bottomRight().y());

//        painter->restore();
//    }

    //绘制外部接口拖入至本控件时的位置状态
    if(isDragging)
    {
        painter->save();
        painter->setPen(QPen(Qt::red,2,Qt::SolidLine));

        switch(dragMoveDirect)
        {
            case DRAG_LEFT:
                             painter->drawLine(dragMovePoint.x(),dragMovePoint.y(),dragMovePoint.x()+CROSS_RADIUS,dragMovePoint.y());
                             painter->drawLine(dragMovePoint.x(),dragMovePoint.y()-CROSS_RADIUS,dragMovePoint.x(),dragMovePoint.y()+CROSS_RADIUS);
                             break;
            case DRAG_TOP:
                             painter->drawLine(dragMovePoint.x()-CROSS_RADIUS,dragMovePoint.y(),dragMovePoint.x()+CROSS_RADIUS,dragMovePoint.y());
                             painter->drawLine(dragMovePoint.x(),dragMovePoint.y(),dragMovePoint.x(),dragMovePoint.y()+CROSS_RADIUS);
                             break;
            case DRAG_RIGHT:
                             painter->drawLine(dragMovePoint.x()-CROSS_RADIUS,dragMovePoint.y(),dragMovePoint.x(),dragMovePoint.y());
                             painter->drawLine(dragMovePoint.x(),dragMovePoint.y()-CROSS_RADIUS,dragMovePoint.x(),dragMovePoint.y()+CROSS_RADIUS);
                             break;
            case DRAG_BOTTOM:
                             painter->drawLine(dragMovePoint.x()-CROSS_RADIUS,dragMovePoint.y(),dragMovePoint.x()+CROSS_RADIUS,dragMovePoint.y());
                             painter->drawLine(dragMovePoint.x(),dragMovePoint.y()-CROSS_RADIUS,dragMovePoint.x(),dragMovePoint.y());
                             break;
            default:
                 break;
        }
        painter->restore();
    }

    painter->restore();
}

void MyItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->buttons() == Qt::LeftButton && CurrAddGraType != GRA_LINE && CurrAddGraType != GRA_VECTOR_LINE)
    {
        setDragPointVisible(true);
    }

    QGraphicsPolygonItem::mousePressEvent(event);
}

void MyItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    //【修复移动控件后，修改样式位置发生微小偏移】
    getRealTimePos();
    emit updateSceneDraw();

    QGraphicsPolygonItem::mouseMoveEvent(event);
}

void MyItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    getRealTimePos();
    emit updateSceneDraw();

    QGraphicsPolygonItem::mouseReleaseEvent(event);
}

//双击编辑文字信息
void MyItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    MY_BUILD_MODEL_ONLY
    emit editMe();
    QGraphicsPolygonItem::mouseDoubleClickEvent(event);
}

//在鼠标事件中获取实时的位置并更新至右侧面板
void MyItem::getRealTimePos()
{
    property.itemRect.x = this->x();
    property.itemRect.y = this->y();
    property.itemRect.width = this->boundingRect().width();
    property.itemRect.height = this->boundingRect().height();

    emit propHasChanged(property);
}

//鼠标进入控件
void MyItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    if(GlobalWindowState == WINDOW_BUILD_MODEL)
    {
        if(CurrAddGraType == GRA_LINE || CurrAddGraType == GRA_VECTOR_LINE)
        {
            isPrepareLine = true;
            update();
        }
        if(!isSelected())
        {
            setDragLineVisible(true);
        }

        setCursor(Qt::SizeAllCursor);
    }
    QGraphicsPolygonItem::hoverEnterEvent(event);
}

void MyItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    if(GlobalWindowState == WINDOW_BUILD_MODEL)
    {
        if(CurrAddGraType == GRA_LINE || CurrAddGraType == GRA_VECTOR_LINE)
        {
            isPrepareLine = false;
            update();
        }
        if(!isSelected())
        {
            setDragLineVisible(false);
        }
        setCursor(Qt::ArrowCursor);
    }

    QGraphicsPolygonItem::hoverLeaveEvent(event);
}

//右键菜单事件
void MyItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    isPrepareLine = false;
    setSelected(true);

    emit clearOtherSelectedItems(this);

    update();

    if(GlobalWindowState == WINDOW_SIMULATE)
    {
        if(currItemType == GRA_ROUND_RECT)
        {
            return;
        }
    }

    MenuManager::instance()->menu(Constants::MENU_ITEM_RIGHT_MENU)->exec(event->screenPos());
}

//拖入事件(注释/并行不允许拖入)
void MyItem::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
    if(event->mimeData()->hasFormat("MyItem"))
    {
        if(currItemType == GRA_ANNOTATION || currItemType == GAR_PARALLE)
        {
             event->ignore();
             return;
        }
        QByteArray array = event->mimeData()->data("MyItem");
        QDataStream stream(&array, QIODevice::ReadOnly);//流，读数据

        int graphicsType;
        stream>>graphicsType;

        if(graphicsType == GRA_NODE_PORT || graphicsType == GRA_NODE_TRIANGLE_OUT || graphicsType == GRA_NODE_HALF_CIRCLE
                || graphicsType == GRA_NODE_TRIANGLE_OUT || graphicsType == GRA_NODE_TRIANGLE_IN || graphicsType ==GRA_NODE_CIRCLE)
        {
            dragGraphicsType = (GraphicsType)graphicsType;
            setSelected(true);
            event->acceptProposedAction();
            isDragging = false;
        }
        else
        {
            dragGraphicsType = GRA_NONE;
            isDragging = false;
            event->ignore();
        }
    }
    else
    {
        setSelected(false);
        isDragging = false;
        event->ignore();
    }
    update();
}

//拖入移动
void MyItem::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
{
    if(event->mimeData()->hasFormat("MyItem"))
    {
        if(getPointToRectMinDistance(boundRect,event->pos()) <= ALLOW_DROP_RANGE)
        {
            isDragging = true;

            dragMoveDirect = getDropDirect(event->pos());

            switch (dragMoveDirect)
            {
                case DRAG_NONE:
                                  return;
                              break;
                case DRAG_LEFT:
                                  dragMovePoint.setX(boundRect.topLeft().x());
                                  dragMovePoint.setY(event->pos().y());
                              break;
                case DRAG_TOP:
                                  dragMovePoint.setX(event->pos().x());
                                  dragMovePoint.setY(boundRect.topLeft().y());
                              break;
                case DRAG_RIGHT:
                                  dragMovePoint.setX(boundRect.topRight().x());
                                  dragMovePoint.setY(event->pos().y());
                              break;
                case DRAG_BOTTOM:
                                  dragMovePoint.setX(event->pos().x());
                                  dragMovePoint.setY(boundRect.bottomRight().y());
                              break;
                default:
                     break;
            }
            setSelected(true);
            event->acceptProposedAction();
        }
        else
        {
            isDragging = false;
            setSelected(false);
            event->ignore();
        }
    }
    else
    {
        isDragging = false;
        setSelected(false);
        event->ignore();
    }
    update();
}

//计算点到矩形四边最小距离
qreal MyItem::getPointToRectMinDistance(QRectF rect,QPointF point)
{
    //左侧
    qreal minDis = 100000;

    qreal leftDis = qAbs(rect.topLeft().x() - point.x());
    minDis = minDis <= leftDis ? minDis:leftDis;

    qreal topDis = qAbs(rect.topLeft().y() - point.y());
    minDis = minDis <= topDis ? minDis:topDis;

    qreal rightDis = qAbs(rect.bottomRight().x() - point.x());
    minDis = minDis <= rightDis ? minDis:rightDis;

    qreal bottomDis = qAbs(rect.bottomRight().y() - point.y());
    minDis = minDis <= bottomDis ? minDis:bottomDis;

    return minDis;
}

//拖入离开
void MyItem::dragLeaveEvent(QGraphicsSceneDragDropEvent *event)
{
    isDragging = false;
    setSelected(false);
    event->ignore();
    update();
}

/*!
*拖拽放下
*在放下时候需要考虑放在哪一侧,
*!*/
void MyItem::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    if(event->mimeData()->hasFormat("MyItem"))
    {
        QPointF dropPoint = event->pos();
        QPointF itemPos;
        qreal scaleFactor;

        DragDirect direct = getDropDirect(dropPoint);

        switch (direct)
        {
            case DRAG_NONE:
                              return;
                          break;
            case DRAG_LEFT:
                              itemPos.setX(boundRect.topLeft().x());
                              itemPos.setY(dropPoint.y());
                              scaleFactor = (dropPoint.y() - boundRect.topLeft().y()) / boundRect.height();
                          break;
            case DRAG_TOP:
                              itemPos.setX(dropPoint.x());
                              itemPos.setY(boundRect.topLeft().y());
                              scaleFactor = (dropPoint.x() - boundRect.topLeft().x()) / boundRect.width();
                          break;
            case DRAG_RIGHT:
                              itemPos.setX(boundRect.bottomRight().x());
                              itemPos.setY(dropPoint.y());
                              scaleFactor = (dropPoint.y() - boundRect.topRight().y()) / boundRect.height();
                          break;
            case DRAG_BOTTOM:
                              itemPos.setX(dropPoint.x());
                              itemPos.setY(boundRect.bottomRight().y());
                              scaleFactor = (dropPoint.x() - boundRect.bottomLeft().x()) / boundRect.width();
                          break;
            default:
                 break;
        }

        createProp(dragGraphicsType,itemPos,direct,scaleFactor);

        setSelected(false);

        isDragging = false;
    }
}

//获取拖放在四边的位置
DragDirect MyItem::getDropDirect(const QPointF &currPoint)
{
    DragDirect direct;
    //放在左侧
    if(qAbs(currPoint.x() - boundRect.topLeft().x()) <=  ALLOW_DROP_RANGE)
    {
        direct = DRAG_LEFT;
    }
    //放在上侧
    else if(qAbs(currPoint.y() - boundRect.topLeft().y()) <=  ALLOW_DROP_RANGE)
    {
        direct = DRAG_TOP;
    }
    //放在右侧
    else if(qAbs(currPoint.x() - boundRect.bottomRight().x()) <=  ALLOW_DROP_RANGE)
    {
        direct = DRAG_RIGHT;
    }
    //放在下侧
    else if(qAbs(currPoint.y() - boundRect.bottomRight().y()) <=  ALLOW_DROP_RANGE)
    {
        direct = DRAG_BOTTOM;
    }
    else
    {
        direct = DRAG_NONE;
    }
    return direct;
}

//拷贝或者拖入一个端口
MyNodePort * MyItem::addNodePort(const NodePortProperty &prop)
{
    QPointF itemPos;
    switch(prop.direct)
    {
        case DRAG_LEFT:
                    {
                        itemPos.setX(boundRect.topLeft().x());
                        itemPos.setY(prop.scaleFactor*boundRect.height()+boundRect.topLeft().y());
                    }
                    break;
        case DRAG_TOP:
                    {
                        itemPos.setX(prop.scaleFactor*boundRect.width()+boundRect.topLeft().x());
                        itemPos.setY(boundRect.topLeft().y());
                    }
                    break;
        case DRAG_RIGHT:
                    {
                        itemPos.setX(boundRect.topRight().x());
                        itemPos.setY(prop.scaleFactor*boundRect.height()+boundRect.topRight().y());
                    }
                    break;
        case DRAG_BOTTOM:
                    {
                        itemPos.setX(prop.scaleFactor*boundRect.width()+boundRect.bottomLeft().x());
                        itemPos.setY(boundRect.bottomLeft().y());
                    }
                    break;
        default:
             break;
    }

    MyNodePort * nodePort = createProp(prop.portType,itemPos,prop.direct,prop.scaleFactor);

    nodePort->setText(prop.content);

    return nodePort;
}

MyNodePort * MyItem::createProp(GraphicsType  type, const QPointF pos,const DragDirect direct,const qreal scaleFactor)
{
    MyNodePort * port = new MyNodePort(type,this);
    port->setPos(pos);
    port->setDragDirect(direct);
    port->setScaleFactor(scaleFactor);
    port->setProperty(property);

    ports.push_back(port);

    return port;
}

//移除端口
void MyItem::removeNodePort(MyNodePort *port)
{
    if(ports.size() > 0)
    {
        ports.removeOne(port);
    }
}

//处理端口位置改变,确保端口只可以在边上移动
void MyItem::procPortChanged(MouseType type, QPointF currPoint)
{
    MyNodePort * tmpPort = qobject_cast<MyNodePort *>(QObject::sender());

    if(tmpPort)
    {
        if(type == MOUSE_MOVE)
        {
            QPointF newPoint;
            qreal tmpX,tmpY;
            DragRange  range = DRAG_RANG_NONE;

            switch(tmpPort->getDragDirect())
            {
                case DRAG_LEFT:
                              {
                                   tmpX = boundRect.topLeft().x();
                                   tmpY = tmpPort->pos().y() + currPoint.y();
                                   //到达线段的边界
                                   bool flag = tmpPort->isArrivalLimitRang();
                                   //到达边界后，
                                   if(flag)
                                   {
                                       if(currPoint.x()>=MOVE_LIMIT_RANG)
                                       {
                                           if(tmpPort->getNextDirect() == DRAG_TOP)
                                           {
                                               tmpPort->setDragDirect(DRAG_TOP);
                                               tmpPort->setPos(boundRect.topLeft());
                                           }
                                           else if(tmpPort->getNextDirect() == DRAG_BOTTOM)
                                           {
                                               tmpPort->setDragDirect(DRAG_BOTTOM);
                                               tmpPort->setPos(boundRect.bottomLeft());
                                           }
                                       }
                                       else
                                       {
                                           tmpPort->setArrivalLimitRang(false);
                                       }
                                       return;
                                   }
                                   else
                                   {
                                       if(getRangeValue(boundRect.bottomLeft().y(),boundRect.topLeft().y(),tmpY,range))
                                       {
                                           if(range == DRAG_RANG_MAX)
                                           {
                                               tmpPort->setNextDirect(DRAG_BOTTOM);
                                           }
                                           else if(range == DRAG_RANG_MIN)
                                           {
                                               tmpPort->setNextDirect(DRAG_TOP);
                                           }
                                       }
                                   }
                              }
                               break;
                case DRAG_TOP:
                              {
                                   tmpX = tmpPort->pos().x() + currPoint.x();
                                   tmpY = boundRect.topLeft().y();

                                   //到达线段的边界
                                   bool flag = tmpPort->isArrivalLimitRang();
                                   //到达边界后，
                                   if(flag)
                                   {
                                       if(currPoint.y()>=MOVE_LIMIT_RANG)
                                       {
                                           if(tmpPort->getNextDirect() == DRAG_RIGHT)
                                           {
                                               tmpPort->setDragDirect(DRAG_RIGHT);
                                               tmpPort->setPos(boundRect.topRight());
                                           }
                                           else if(tmpPort->getNextDirect() == DRAG_LEFT)
                                           {
                                               tmpPort->setDragDirect(DRAG_LEFT);
                                               tmpPort->setPos(boundRect.topLeft());
                                           }
                                       }
                                       else
                                       {
                                           tmpPort->setArrivalLimitRang(false);
                                       }
                                       return;
                                   }
                                   else
                                   {
                                       if(getRangeValue(boundRect.topRight().x(),boundRect.topLeft().x(),tmpX,range))
                                       {
                                           if(range == DRAG_RANG_MAX)
                                           {
                                               tmpPort->setNextDirect(DRAG_RIGHT);
                                           }
                                           else if(range == DRAG_RANG_MIN)
                                           {
                                               tmpPort->setNextDirect(DRAG_LEFT);
                                           }
                                       }
                                   }
                              }
                               break;
                case DRAG_RIGHT:
                              {
                                   tmpX = boundRect.topRight().x();
                                   tmpY = tmpPort->pos().y() + currPoint.y();
                                   //到达线段的边界
                                   bool flag = tmpPort->isArrivalLimitRang();
                                   //到达边界后，
                                   if(flag)
                                   {
                                       if(-currPoint.x()>=MOVE_LIMIT_RANG)
                                       {
                                           if(tmpPort->getNextDirect() == DRAG_BOTTOM)
                                           {
                                               tmpPort->setDragDirect(DRAG_BOTTOM);
                                               tmpPort->setPos(boundRect.bottomRight());
                                           }
                                           else if(tmpPort->getNextDirect() == DRAG_TOP)
                                           {
                                               tmpPort->setDragDirect(DRAG_TOP);
                                               tmpPort->setPos(boundRect.topRight());
                                           }
                                       }
                                       else
                                       {
                                           tmpPort->setArrivalLimitRang(false);
                                       }
                                       return;
                                   }
                                   else
                                   {
                                       if(getRangeValue(boundRect.bottomRight().y(),boundRect.topRight().y(),tmpY,range))
                                       {
                                           if(range == DRAG_RANG_MAX)
                                           {
                                               tmpPort->setNextDirect(DRAG_BOTTOM);
                                           }
                                           else if(range == DRAG_RANG_MIN)
                                           {
                                               tmpPort->setNextDirect(DRAG_TOP);
                                           }
                                       }
                                   }
                              }
                               break;
                case DRAG_BOTTOM:
                              {
                                   tmpX = tmpPort->pos().x() + currPoint.x();
                                   tmpY = boundRect.bottomLeft().y();

                                   //到达线段的边界
                                   bool flag = tmpPort->isArrivalLimitRang();
                                   //到达边界后，
                                   if(flag)
                                   {
                                       if(-currPoint.y()>=MOVE_LIMIT_RANG)
                                       {
                                           if(tmpPort->getNextDirect() == DRAG_RIGHT)
                                           {
                                               tmpPort->setDragDirect(DRAG_RIGHT);
                                               tmpPort->setPos(boundRect.bottomRight());
                                           }
                                           else if(tmpPort->getNextDirect() == DRAG_LEFT)
                                           {
                                               tmpPort->setDragDirect(DRAG_LEFT);
                                               tmpPort->setPos(boundRect.bottomLeft());
                                           }
                                       }
                                       else
                                       {
                                           tmpPort->setArrivalLimitRang(false);
                                       }
                                       return;
                                   }
                                   else
                                   {
                                       if(getRangeValue(boundRect.bottomRight().x(),boundRect.bottomLeft().x(),tmpX,range))
                                       {
                                           if(range == DRAG_RANG_MAX)
                                           {
                                               tmpPort->setNextDirect(DRAG_RIGHT);
                                           }
                                           else if(range == DRAG_RANG_MIN)
                                           {
                                               tmpPort->setNextDirect(DRAG_LEFT);
                                           }
                                       }
                                   }
                              }
                               break;
                default:
                    break;
            }

            newPoint.setX(tmpX);
            newPoint.setY(tmpY);

            tmpPort->setPos(newPoint);
        }
        else if(type == MOUSE_RELEASE)
        {
            qreal scaleFactor;
            switch(tmpPort->getDragDirect())
            {
                case DRAG_LEFT:
                              {
                                   scaleFactor = (tmpPort->pos().y() - boundRect.topLeft().y()) / boundRect.height();
                              }
                               break;
                case DRAG_TOP:
                              {
                                   scaleFactor = (tmpPort->pos().x() - boundRect.topLeft().x()) / boundRect.width();
                              }
                               break;
                case DRAG_RIGHT:
                              {
                                   scaleFactor = (tmpPort->pos().y() - boundRect.topRight().y()) / boundRect.height();
                              }
                               break;
                case DRAG_BOTTOM:
                              {
                                   scaleFactor = (tmpPort->pos().x() - boundRect.topLeft().x()) / boundRect.width();
                              }
                               break;
                default:
                     break;
            }

            tmpPort->setScaleFactor(scaleFactor);
        }

        tmpPort->updatePathQuoteType();
        emit updateSceneDraw();
    }
}

//删除某个端口
void MyItem::procDeleteNodePort(MyNodePort *nodePort)
{
    int index = -1;
    for(int i = 0;i<ports.size();i++)
    {
        if(ports.at(i) ==  nodePort)
        {
            index = i;
            break;
        }
    }

    if(index >= 0)
    {
        ports.at(index)->removeArrows();

        delete ports.at(index);
        ports.removeAt(index);
    }
}

//编辑某个端口
void MyItem::procEditNodePort(MyNodePort *)
{
    emit editItemPort();
//    MyGraphicsView::instance()->showNodePortEdit(nodePort);
}

//将当前值和最大及最小值相比较，只能在此范围内,到达最大或最小值的边界时，返回用于设置改变方向
bool MyItem::getRangeValue(qreal maxValue, qreal minValue, qreal &currValue, DragRange& range)
{
    if(currValue >= maxValue)
    {
        currValue = maxValue;
        range = DRAG_RANG_MAX;
        return true;
    }
    else if(currValue <= minValue)
    {
        currValue = minValue;
        range = DRAG_RANG_MIN;
        return true;
    }
    return false;
}

//设置是否拖拽点和旋转点是否可见
void MyItem::setDragPointVisible(bool flag)
{
    leftTopPoint->setVisible(flag);
    rightTopPoint->setVisible(flag);
    leftBottomPoint->setVisible(flag);
    rightBottomPoint->setVisible(flag);

    rotateLine->setVisible(flag);

    setDragLineVisible(flag);
}

//设置连接线拖拽点是否可见
void MyItem::setDragLineVisible(bool isVisible)
{
    topLinePoint->setVisible(isVisible);
    leftLinePoint->setVisible(isVisible);
    rightLinePoint->setVisible(isVisible);
    bottomLinePoint->setVisible(isVisible);

    if(currItemType == GRA_ANNOTATION)
    {
        topLinePoint->setVisible(false);
        rightLinePoint->setVisible(false);
        bottomLinePoint->setVisible(false);
    }
    else if(currItemType == GAR_PARALLE)
    {
        leftLinePoint->setVisible(false);
        rightLinePoint->setVisible(false);
    }
}

QVariant MyItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if(change == ItemSelectedChange && scene())
    {
        setDragPointVisible(true);
        isNeedBorder = true;
    }
    else if(change == ItemSelectedHasChanged && scene())
    {
        if(!isSelected()&&(currMouseType == MOUSE_NONE||currMouseType == MOUSE_RELEASE))
        {
            setDragPointVisible(false);
            isNeedBorder = false;
        }
    }
    else if(change == QGraphicsItem::ItemRotationHasChanged && scene())
    {
        updateDragPointCursor();
        updateLinePos();
        emit itemRotationChanged(this);
    }
    else if(change == QGraphicsItem::ItemPositionHasChanged)
    {
        emit itemPosChanged(this);
        updateLinePos();
    }
    return QGraphicsPolygonItem::itemChange(change,value);
}

//更新拖拽点的鼠标样式
void MyItem::updateDragPointCursor()
{
    leftTopPoint->updateDragPointHoverCursor(rotation());
    rightTopPoint->updateDragPointHoverCursor(rotation());
    leftBottomPoint->updateDragPointHoverCursor(rotation());
    rightBottomPoint->updateDragPointHoverCursor(rotation());
}

//处理操作线段拖拽点鼠标状态
void MyItem::procDragLineMouseState(MouseType type)
{
    currMouseType = type;
    if(currMouseType == MOUSE_NONE || currMouseType == MOUSE_RELEASE)
    {
        setDragPointVisible(false);
    }
}

//左上、右上、左下、右下在缩放时要按照比例进行
//上、左、右、下在缩放时只按改变一边长度
void MyItem::procMouseState(MouseType type,PointType pointType,QPointF currPos)
{

    if(!property.isMoveable)
    {
        return;
    }

    currMouseType = type;

    if(currMouseType == MOUSE_PRESS)
    {

    }
    else if(currMouseType == MOUSE_MOVE)
    {
        qreal leftTopX = mapToScene(boundRect.topLeft()).x();
        qreal leftTopY = mapToScene(boundRect.topLeft()).y();

        qreal rightTopX = mapToScene(boundRect.topRight()).x();
        qreal rightTopY = mapToScene(boundRect.topRight()).y();

        qreal rightBottomX = mapToScene(boundRect.bottomRight()).x();
        qreal rightBottomY = mapToScene(boundRect.bottomRight()).y();

        qreal leftBottomX = mapToScene(boundRect.bottomLeft()).x();
        qreal leftBottomY = mapToScene(boundRect.bottomLeft()).y();

        qreal w = boundRect.width();
        qreal h = boundRect.height();

        //鼠标移动当前坐标值
        qreal px = currPos.x();
        qreal py = currPos.y();
        //移动后x、y、w、h
        qreal tmpX,tmpY,tmpW,tmpH;
        //移动后中心点的x、y
        qreal centerX,centerY;
        //当前旋转的角度
        qreal rotateDegree = rotation();

        //【20160930控件四角不按比例缩放】
//        qreal  factor = w / h;

        QPainterPath path;

        bool hasProcessed = false;                 //选中的控件在拖拽时是否可用

        switch(pointType)
        {
            //控件四个角上点
            case TOP_LEFT:
            case TOP_RIGHT:
            case BOTTOM_LEFT:
            case BOTTOM_RIGHT:
                {
                    if(pointType ==  TOP_LEFT)
                    {
                        tmpW = w - px;
                        tmpH = h - py;

                        tmpW = Util::getMaxNum(tmpW,GlobalMinMumSize);
                        tmpH = Util::getMaxNum(tmpH,GlobalMinMumSize);

                        tmpX = tmpW/2;
                        tmpY = tmpH/2;

                        qreal beta = rotateDegree + Util::radinToAngle(qAtan(tmpH/tmpW));
                        centerX = rightBottomX - sqrt(qPow(tmpH,2) +qPow(tmpW,2)) * QCOS(beta)/2;
                        centerY = rightBottomY - sqrt(qPow(tmpH,2) +qPow(tmpW,2)) * QSIN(beta)/2;

                        prepareGeometryChange();
                        boundRect = QRectF(-tmpX,-tmpY,tmpW,tmpH);
                    }
                    else if(pointType ==  TOP_RIGHT)
                    {
                        tmpW = w + px;
                        tmpH = h - py;

                        tmpW = Util::getMaxNum(tmpW,GlobalMinMumSize);
                        tmpH = Util::getMaxNum(tmpH,GlobalMinMumSize);

//                        tmpH = tmpW /factor;
                        tmpX = tmpW/2;
                        tmpY = tmpH/2;

                        qreal beta = 90 - rotateDegree + Util::radinToAngle(qAtan(tmpH/tmpW));
                        centerX = leftBottomX + sqrt(qPow(tmpH,2) +qPow(tmpW,2)) * QSIN(beta)/2;
                        centerY = leftBottomY + sqrt(qPow(tmpH,2) +qPow(tmpW,2)) * QCOS(beta)/2;

                        prepareGeometryChange();
                        boundRect = QRectF(-tmpX,-tmpY,tmpW,tmpH);
                    }
                    else if(pointType ==  BOTTOM_LEFT)
                    {
                        tmpW = w - px;
                        tmpH = h + py;

                        tmpW = Util::getMaxNum(tmpW,GlobalMinMumSize);
                        tmpH = Util::getMaxNum(tmpH,GlobalMinMumSize);

//                        tmpH = tmpW /factor;
                        tmpX = tmpW/2;
                        tmpY = tmpH/2;
                        qreal beta = 90 - rotateDegree + Util::radinToAngle(qAtan(tmpH/tmpW));
                        centerX = rightTopX - sqrt(qPow(tmpH,2) +qPow(tmpW,2)) * QSIN(beta)/2;
                        centerY = rightTopY - sqrt(qPow(tmpH,2) +qPow(tmpW,2)) * QCOS(beta)/2;

                        prepareGeometryChange();
                        boundRect = QRectF(-tmpX,-tmpY,tmpW,tmpH);
                    }
                    else if(pointType ==  BOTTOM_RIGHT)
                    {
                        tmpW = w + px;
                        tmpH = h + py;

                        tmpW = Util::getMaxNum(tmpW,GlobalMinMumSize);
                        tmpH = Util::getMaxNum(tmpH,GlobalMinMumSize);

//                        tmpH = tmpW /factor;
                        tmpX = tmpW/2;
                        tmpY = tmpH/2;

                        qreal beta = rotateDegree + Util::radinToAngle(qAtan(tmpH/tmpW));
                        centerX = leftTopX + sqrt(qPow(tmpH,2) +qPow(tmpW,2)) * QCOS(beta)/2;
                        centerY = leftTopY + sqrt(qPow(tmpH,2) +qPow(tmpW,2)) * QSIN(beta)/2;

                        prepareGeometryChange();
                        boundRect = QRectF(-tmpX,-tmpY,tmpW,tmpH);
                    }

                    switch(currItemType)
                    {
                        case GRA_SQUARE:
                        case GRA_RECT:
                        case GRA_ANNOTATION:
                        case GAR_PARALLE:
#ifdef ADD_STATE_MODEL
                        case GRA_MASK_RECT:
                        case GRA_NODE_PROCESS:
#endif
                            itemPolygon.clear();
                            itemPolygon<<QPointF(-tmpX,-tmpY)<<QPointF(tmpX,-tmpY)<<
                                    QPointF(tmpX,tmpY)<<QPointF(-tmpX,tmpY);
                            hasProcessed = true;
                            break;

                        case GRA_ROUND_RECT:
#ifdef ADD_STATE_MODEL
                        case GRA_STATE_PROCESS:
                        case GRA_MASK_BOUND_RECT:
#endif
                            itemPolygon.clear();

                            path.addRoundedRect(boundRect,10,10);
                            itemPolygon = path.toFillPolygon();
                            hasProcessed = true;
                            break;

                        case GRA_CIRCLE:
#ifdef ADD_STATE_MODEL
                        case GRA_STATE_START:
                        case GRA_MASK_CIRCLE:
#endif
                            itemPolygon.clear();
                            path.addEllipse(boundRect);
                            itemPolygon = path.toFillPolygon();
                            hasProcessed = true;
                            break;
#ifdef ADD_STATE_MODEL
                        case GRA_STATE_END:
                            itemPolygon.clear();
                            path.addEllipse(QRectF(boundRect.x() + STATE_END_RADIUS,
                                                   boundRect.y()+STATE_END_RADIUS,boundRect.width()-STATE_END_RADIUS*2,boundRect.height()-STATE_END_RADIUS*2));
                            itemPolygon = path.toFillPolygon();
                            hasProcessed = true;
                            break;
#endif
                        case GRA_ELLIPSE:
                            itemPolygon.clear();
                            path.addEllipse(boundRect);
                            itemPolygon = path.toFillPolygon();
                            hasProcessed = true;
                            break;
                        case GRA_POLYGON:
                            itemPolygon.clear();
                            itemPolygon<<QPointF(-tmpX,0)<<QPointF(0,-tmpY)<<
                                    QPointF(tmpX,0)<<QPointF(0,tmpY);
                            hasProcessed = true;
                            break;
                        case GRA_PARALLELOGRAM:
                            itemPolygon.clear();
                            itemPolygon<<QPointF(-POINT_FIVE*tmpX,-tmpY)<<QPointF(tmpX,-tmpY)<<
                                    QPointF(POINT_FIVE*tmpX,tmpY)<<QPointF(-tmpX,tmpY);
                            hasProcessed = true;
                            break;
                        case GRA_LOOP_UP:
                            {
                                itemPolygon.clear();

                                qreal loopWith = getLoopMaxSidLength(tmpW,tmpH);

                                itemPolygon<<QPointF(-tmpX+loopWith,-tmpY)<<QPointF(tmpX-loopWith,-tmpY)
                                            <<QPointF(tmpX,-tmpY+loopWith)<<QPointF(tmpX,tmpY)
                                             <<QPointF(-tmpX,tmpY)<<QPointF(-tmpX,-tmpY+loopWith);
                                hasProcessed = true;
                            }
                            break;
                        case GRA_LOOP_DOWN:
                            {
                                itemPolygon.clear();

                                qreal loopWith = getLoopMaxSidLength(tmpW,tmpH);

                                itemPolygon<<QPointF(-tmpX,-tmpY)<<QPointF(tmpX,-tmpY)
                                            <<QPointF(tmpX,tmpY-loopWith)<<QPointF(tmpX-loopWith,tmpY)
                                             <<QPointF(-tmpX+loopWith,tmpY)<<QPointF(-tmpX,tmpY-loopWith);
                                hasProcessed = true;
                            }
                            break;
                        default:
                             break;
                    }
                    break;
                }
            default:
                 break;
        }

        if(hasProcessed)
        {
            setPos(QPointF(centerX,centerY));
            setPolygon(itemPolygon);
            procResizeItem();
            updateRotateLinePos();
            procResizeNodePort();

            property.itemRect.x = mapToScene(boundRect.topLeft()).x()+boundRect.width()/2;
            property.itemRect.y = mapToScene(boundRect.topLeft()).y()+boundRect.height()/2;
            property.itemRect.width = boundRect.width();
            property.itemRect.height = boundRect.height();

            emit propHasChanged(property);
        }
    }
    else if(currMouseType == MOUSE_RELEASE)
    {
        setSelected(true);
    }
    emit updateSceneDraw();
}

//调整8个拖拽矩形的位置
void MyItem::procResizeItem()
{
    leftTopPoint->setPos(boundRect.topLeft());
    rightTopPoint->setPos(boundRect.topRight());
    leftBottomPoint->setPos(boundRect.bottomLeft());
    rightBottomPoint->setPos(boundRect.bottomRight());

    if(currItemType == GRA_PARALLELOGRAM)
    {
        topLinePoint->setPos(0/*(itemPolygon.at(0).x() + itemPolygon.at(1).x())/2*/, itemPolygon.at(0).y());
        rightLinePoint->setPos((itemPolygon.at(1).x() + itemPolygon.at(2).x())/2, 0);
        bottomLinePoint->setPos(0/*(itemPolygon.at(0).x() + itemPolygon.at(1).x())/2*/, itemPolygon.at(2).y());
        leftLinePoint->setPos((itemPolygon.at(3).x() + itemPolygon.at(0).x())/2, 0);
    }
    else
    {
        topLinePoint->setPos(QPointF(0,boundRect.topLeft().y()));
        leftLinePoint->setPos(QPointF(boundRect.topLeft().x(),0));
        rightLinePoint->setPos(QPointF(boundRect.bottomRight().x(),0));
        bottomLinePoint->setPos(QPointF(0,boundRect.bottomLeft().y()));
    }
}

//调整拖入端口的位置
void MyItem::procResizeNodePort()
{
    QPointF newPoint;
    foreach (MyNodePort * nodePort, ports)
    {
        switch(nodePort->getDragDirect())
        {
            case DRAG_LEFT:
                          {
                               newPoint.setX(boundRect.topLeft().x());
                               newPoint.setY(nodePort->getScaleFactor()*boundRect.height() + boundRect.topLeft().y());
                          }
                           break;
            case DRAG_TOP:
                          {
                               newPoint.setX(nodePort->getScaleFactor()*boundRect.width() + boundRect.topLeft().x());
                               newPoint.setY(boundRect.topLeft().y());
                          }
                           break;
            case DRAG_RIGHT:
                          {
                               newPoint.setX(boundRect.topRight().x());
                               newPoint.setY(nodePort->getScaleFactor()*boundRect.height() + boundRect.topRight().y());
                          }
                           break;
            case DRAG_BOTTOM:
                          {
                               newPoint.setX(nodePort->getScaleFactor()*boundRect.width() + boundRect.bottomLeft().x());
                               newPoint.setY(boundRect.bottomLeft().y());
                          }
                           break;
            default:
                 break;
        }

        nodePort->setPos(newPoint);
    }
}

//处理旋转控件旋转后角度的设置
void MyItem::procRotate(MouseType mouseType,qreal degree)
{
    currMouseType = mouseType;
    int tempDegree = static_cast<int>(degree);

    if(currMouseType == MOUSE_MOVE)
    {
        if(tempDegree % GlobalRotateStep == 0)
        {
            setRotation(tempDegree);
            property.rotateDegree = tempDegree;
        }
    }
    else if(currMouseType == MOUSE_RELEASE)
    {
        setSelected(true);
    }

    emit propHasChanged(property);
}

//设置控件的样式属性
void MyItem::setProperty(ItemProperty property)
{
    this->property = property;
    setBrush(property.itemBrush);

    setRotation(property.rotateDegree);

    updateDragPointCursor();

    setZValue(property.zValue);
    setPos(QPointF (property.itemRect.x,property.itemRect.y));

    resetPolygon();

    myTextItem->setPlainText(property.content);
    myTextItem->updateFont(property.itemFont);
    myTextItem->setDefaultTextColor(property.fontColor);
    setText(myTextItem->toPlainText());

    setFlag(QGraphicsItem::ItemIsMovable,property.isMoveable);

    foreach (MyNodePort * node, ports)
    {
        node->setProperty(property);
    }

    rotateLine->setMoveable(property.isMoveable);

    parentScene->update();
}

//本地打开文件后，对控件进行重新设定
void MyItem::resetPolygon()
{
    qreal tw = this->property.itemRect.width;
    qreal th = this->property.itemRect.height;
    qreal tx = tw/2;
    qreal ty = th/2;

    prepareGeometryChange();
    boundRect = QRectF(-tx,-ty,tw,th);

    setInitalPolygon(boundRect,tx,ty,tw,th);

    procResizeItem();
    updateRotateLinePos();
    procResizeNodePort();
}

//设置初始的多边形
void MyItem::setInitalPolygon(QRectF boundRect,qreal tx,qreal ty,qreal tw,qreal th)
{
    itemPolygon.clear();

    switch(currItemType)
    {
            //正方形
            case GRA_SQUARE:
                               itemPolygon<<QPointF(-tx,-ty)<<QPointF(tx,-ty)<<
                                      QPointF(tx,ty)<<QPointF(-tx,ty);
                               break;
            //矩形
            case GRA_RECT:
            case GRA_ANNOTATION:
            case GAR_PARALLE:
#ifdef ADD_STATE_MODEL
            case GRA_NODE_PROCESS:
            case GRA_MASK_RECT:
#endif
                               itemPolygon<<QPointF(-tx,-ty)<<QPointF(tx,-ty)<<
                                       QPointF(tx,ty)<<QPointF(-tx,ty);
                               break;
           //圆角矩形/状态处理
           case GRA_ROUND_RECT:
#ifdef ADD_STATE_MODEL
           case GRA_STATE_PROCESS:
           case GRA_MASK_BOUND_RECT:
#endif
                            {
                                QPainterPath path;
                                path.addRoundedRect(boundRect,10,10);
                                itemPolygon = path.toFillPolygon();
                            }
                              break;
            //圆形/状态开始/圆形遮罩
            case GRA_CIRCLE:
#ifdef ADD_STATE_MODEL
            case GRA_STATE_START:
            case GRA_MASK_CIRCLE:
#endif
                            {
                                QPainterPath path;
                                path.addEllipse(boundRect);
                                itemPolygon = path.toFillPolygon();
                            }
                               break;
#ifdef ADD_STATE_MODEL
             //状态结束
             case GRA_STATE_END:
                            {
                                QPainterPath path;
                                path.addEllipse(QRectF(boundRect.x() + STATE_END_RADIUS,
                                                       boundRect.y()+STATE_END_RADIUS,boundRect.width()-STATE_END_RADIUS*2,boundRect.height()-STATE_END_RADIUS*2));
                                itemPolygon = path.toFillPolygon();
                            }
                                break;
#endif
            //椭圆
            case GRA_ELLIPSE:
                            {
                                QPainterPath path;
                                path.addEllipse(boundRect);
                                itemPolygon = path.toFillPolygon();
                            }
                               break;
            //菱形
            case GRA_POLYGON:
                            {
                               itemPolygon<<QPointF(-tx,0)<<QPointF(0,-ty)<<
                                      QPointF(tx,0)<<QPointF(0,ty);
                            }
                               break;
            //平行四边形
            case GRA_PARALLELOGRAM:
                            {
                               itemPolygon<<QPointF(-POINT_FIVE*tx,-ty)<<QPointF(tx,-ty)<<
                                      QPointF(POINT_FIVE*tx,ty)<<QPointF(-tx,ty);
                            }
                               break;
            //循环上限
            case GRA_LOOP_UP:
                            {
                                qreal loopWith = getLoopMaxSidLength(tw,th);
                                itemPolygon<<QPointF(-tx+loopWith,-ty)<<QPointF(tx-loopWith,-ty)
                                            <<QPointF(tx,-ty+loopWith)<<QPointF(tx,ty)
                                             <<QPointF(-tx,ty)<<QPointF(-tx,-ty+loopWith);
                            }
                               break;
            //循环下限
            case GRA_LOOP_DOWN:
                            {
                                qreal loopWith = getLoopMaxSidLength(tw,th);
                                itemPolygon<<QPointF(-tx,-ty)<<QPointF(tx,-ty)
                                            <<QPointF(tx,ty-loopWith)<<QPointF(tx-loopWith,ty)
                                             <<QPointF(-tx+loopWith,ty)<<QPointF(-tx,ty-loopWith);
                            }
                               break;
            default:
                 break;
    }
    setPolygon(itemPolygon);
}

//左旋转或者右旋转后更新当前属性的旋转角度值
void MyItem::updateRotation(int rotateValue)
{
    property.rotateDegree += rotateValue;
    property.rotateDegree = property.rotateDegree%360;
    emit propHasChanged(property);
}

//重新设定控件的UUID编号，同时更新子节点的父索引值【谨慎使用】
void MyItem::resetItemUUID()
{
    property.createUUID();
}

//设置控件状态【锁定状态:不能改变位置、大小、角度、Z值、样式等特征】
void MyItem::setMoveable(bool lockState)
{
    this->property.isMoveable = lockState;

    setFlag(QGraphicsItem::ItemIsMovable,lockState);

    rotateLine->setMoveable(lockState);

    foreach (MyNodePort* node, ports)
    {
        node->setMoveable(lockState);
    }
}

//键盘事件
void MyItem::keyPressEvent(QKeyEvent *event)
{
    QGraphicsPolygonItem::keyPressEvent(event);
}

//计算循环上限的一边的最大长度
qreal MyItem::getLoopMaxSidLength(qreal width,qreal height)
{
    qreal realWidth;
    qreal halfWidth = width / 2;
    qreal halfHeight = height / 2;

    if(halfHeight >= MAX_LOOP_RADIUS)
    {
        realWidth = MAX_LOOP_RADIUS;
    }
    else if(halfWidth >= MAX_LOOP_RADIUS)
    {
        realWidth = MAX_LOOP_RADIUS;
    }
    else if(halfWidth > halfHeight)
    {
        realWidth = halfHeight;
    }
    else
    {
        realWidth = halfWidth;
    }
    return realWidth;
}

//返回四边中点的索引
DragLinePoint * MyItem::getDragLinePoint(PointType pointType)
{
    DragLinePoint * tmp = NULL;
    switch(pointType)
    {
        case TOP_MIDDLE: tmp = topLinePoint;
                               break;
        case MIDDLE_LEFT: tmp = leftLinePoint;
                               break;
        case MIDDLE_RIGHT: tmp = rightLinePoint;
                               break;
        case BOTTOM_MIDDLE: tmp = bottomLinePoint;
                               break;
        default:
             break;
    }
    return tmp;
}

//删除此控件时，需要将此控件中的DragLinePoint上的箭头清除掉
void MyItem::removeDragLineArrows()
{
    topLinePoint->removeArrows();
    leftLinePoint->removeArrows();
    rightLinePoint->removeArrows();
    bottomLinePoint->removeArrows();

    topLinePoint->removePathLines();
    leftLinePoint->removePathLines();
    rightLinePoint->removePathLines();
    bottomLinePoint->removePathLines();
}

//获取当前控件所有的箭头
QList<MyArrow *> MyItem::getArrows()
{
    QList<MyArrow *> arrows;
    arrows.append(topLinePoint->getArrows());
    arrows.append(rightLinePoint->getArrows());
    arrows.append(bottomLinePoint->getArrows());
    arrows.append(leftLinePoint->getArrows());
    return arrows;
}

//获取控件的所有折线
QList<MyPathItem * > MyItem::getPathItems()
{
    QList<MyPathItem *> pathItems;
    pathItems.append(topLinePoint->getPathItems());
    pathItems.append(rightLinePoint->getPathItems());
    pathItems.append(bottomLinePoint->getPathItems());
    pathItems.append(leftLinePoint->getPathItems());
    return pathItems;
}

//设置此控件是否高亮显示
void MyItem::hightLightItem(HightLightLevel level,bool isHigh)
{
    lightLevel = level;
    isSimulateHigh = isHigh;
    update();
}

#ifdef ADD_STATE_MODEL

void MyItem::setStartProp(StateStartProperty &prop)
{
    startProp.content = prop.content;
}

void MyItem::setModelProp(StateModelProperty &prop)
{
    stateModeProp.stateName = prop.stateName;
    stateModeProp.continueContent = prop.continueContent;
    stateModeProp.props.clear();
    foreach(StatInnerProperty tmpProp,prop.props)
    {
        stateModeProp.props.append(tmpProp);
    }
    setText(prop.stateName);
}
#endif

//设置服务的属性
void MyItem::setServiceProperty(ServiceProperty *prop)
{
    serviceProp->hasSettInfo = prop->hasSettInfo;
    serviceProp->id = prop->id;
    serviceProp->serviceName = prop->serviceName;
    serviceProp->status = prop->status;
    serviceProp->descirption = prop->descirption;
    serviceProp->servicePath = prop->servicePath;
    serviceProp->method = prop->method;
    foreach(Parameter * para, prop->inputParas)
    {
        Parameter * p = new Parameter;
        p->pId = para->pId;
        p->pName = para->pName;
        p->pRemark = para->pRemark;
        p->pType = para->pType;
        p->pValue = para->pValue;

        serviceProp->inputParas.append(p);
    }
    foreach(Parameter * para, prop->outputParas)
    {
        Parameter * p = new Parameter;
        p->pId = para->pId;
        p->pName = para->pName;
        p->pRemark = para->pRemark;
        p->pType = para->pType;
        p->pValue = para->pValue;
        serviceProp->outputParas.append(para);
    }
    serviceProp->isAsNextInput = prop->isAsNextInput;
}

//设置循环属性
void MyItem::setLoopProp(LoopProperty *prop)
{
    foreach(SignalVari * tmp,prop->signalList)
    {
        SignalVari * s = new SignalVari;
        s->variName = tmp->variName;
        s->isQuoted = tmp->isQuoted;
        s->isAssignedValue = tmp->isAssignedValue;
        s->isLegal = tmp->isLegal;
        s->initialValue = tmp->initialValue;
        s->finalValue = tmp->finalValue;
        s->middlValue = tmp->middlValue;
        s->operateSymbol = tmp->operateSymbol;
        s->selfOperateSymbol = tmp->selfOperateSymbol;
        s->selfOperateValue = tmp->selfOperateValue;
        loopProperty->signalList.append(s);
    }

    foreach(VariableDefine * tmp,prop->varList)
    {
        VariableDefine * v = new VariableDefine;
        v->type = tmp->type;
        v->name = tmp->name;
        v->value = tmp->value;
        v->pRemark = tmp->pRemark;
        loopProperty->varList.append(v);
    }

    foreach(ExpressDefine * tmp,prop->expList)
    {
        ExpressDefine * v = new ExpressDefine;
        v->name = tmp->name;
        v->expressType = tmp->expressType;
        v->value = tmp->value;
        v->pRemark = tmp->pRemark;
        loopProperty->expList.append(v);
    }

    foreach(FinalExpressDefine * tmp,prop->fexpList)
    {
        FinalExpressDefine * v = new FinalExpressDefine;
        v->name = tmp->name;
        v->expressType = tmp->expressType;
        v->value = tmp->value;
        v->pRemark = tmp->pRemark;
        loopProperty->fexpList.append(v);
    }
}

//设置判断框属性
void MyItem::setJudgeProp(JudgeProperty *prop)
{
    this->judgeProperty->express = prop->express;
}

//当item移动后，同时更新当前item所连接的线条
void MyItem::updateLinePos()
{
    topLinePoint->updateLinePos();
    leftLinePoint->updateLinePos();
    rightLinePoint->updateLinePos();
    bottomLinePoint->updateLinePos();

    foreach (MyNodePort* node, ports)
    {
        node->updateLinePos();
    }

    emit updateSceneDraw();
}

MyItem::~MyItem()
{
    foreach (MyNodePort* node, ports)
    {
        node->removeArrows();
        delete node;
    }
    ports.clear();

    if(leftTopPoint)
    {
        delete leftTopPoint;
        leftTopPoint = NULL;
    }

    if(rightTopPoint)
    {
        delete rightTopPoint;
        rightTopPoint = NULL;
    }

    if(leftBottomPoint)
    {
        delete leftBottomPoint;
        leftBottomPoint = NULL;
    }

    if(rightBottomPoint)
    {
        delete rightBottomPoint;
        rightBottomPoint = NULL;
    }

    if(topLinePoint)
    {
        delete topLinePoint;
        topLinePoint = NULL;
    }

    if(leftLinePoint)
    {
        delete leftLinePoint;
        leftLinePoint = NULL;
    }

    if(rightLinePoint)
    {
        delete rightLinePoint;
        rightLinePoint = NULL;
    }

    if(bottomLinePoint)
    {
        delete bottomLinePoint;
        bottomLinePoint = NULL;
    }

    if(rotateLine)
    {
        delete rotateLine;
        rotateLine = NULL;
    }

    if(myTextItem)
    {
        delete myTextItem;
        myTextItem = NULL;
    }

    if(serviceProp)
    {
        foreach(Parameter * para,serviceProp->inputParas)
        {
            delete para;
        }
        serviceProp->inputParas.clear();;

        foreach(Parameter * para,serviceProp->outputParas)
        {
            delete para;
        }
        serviceProp->outputParas.clear();

        delete serviceProp;
        serviceProp = NULL;
    }

    if(loopProperty)
    {
        foreach(VariableDefine * para,loopProperty->varList)
        {
            delete para;
        }
        loopProperty->varList.clear();

        foreach(ExpressDefine * para,loopProperty->expList)
        {
            delete para;
        }
        loopProperty->expList.clear();

        foreach(FinalExpressDefine * para,loopProperty->fexpList)
        {
            delete para;
        }
        loopProperty->fexpList.clear();

        foreach(SignalVari * para,loopProperty->signalList)
        {
            delete para;
        }
        loopProperty->signalList.clear();

        delete loopProperty;
        loopProperty = NULL;
    }

}

/**
void ShapeLine::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QStyleOptionGraphicsItem op;
    op.initFrom(widget);
    op.state = QStyle::State_None;

    return QGraphicsLineItem::paint(painter, &op, widget);
}
*/

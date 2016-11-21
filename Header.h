/*************************************************
**版  权：RenGu Company
**文件名: Header.h
**作  者: wey       Version: 1.0       Date: 2016.09.01
**描  述: 定义通用的结构体
**Others:
**
**修改历史:
**20160913:wey:调整类型格式定义为qreal
**20160925:wey:增加隐藏控件位置关系SpiltDirect
**20160927:wey:添加线条类型AddLineType
**             添加拖入类型为平行四边形
**20161009:wey:添加属性编辑结构体ServiceProperty
*************************************************/
#ifndef HEADER_H
#define HEADER_H

#define M_VERTION 0x000A        //程序的版本，在保存文件时，要保存当前文件的版本；解析时也要判断
#define M_AUTHOR "南京仁谷系统集成有限公司"

//用于控制是否需要加入状态机模块，如果不需要，将此宏取消定义
#ifndef ADD_STATE_MODEL
#define ADD_STATE_MODEL
#endif

#define PI 3.141592653
#define ICON_WIDTH  60          //拖拽图标的宽高
#define ICON_HEIGHT 60
#define LAYOUT_SPACING  1       //布局管理器中两控件间的距离
#define FONT_DEAFULT_PIX  9     //字体初始化大小
#define ROTATE_MIN_DEGREE 0     //旋转的最小角度
#define ROTATE_MAX_DEGREE 360   //旋转的最大角度

#define POP_SUB_DIALOG_WIDTH  650    //复合组件弹出子窗口固定宽度
#define POP_SUB_DIALOG_HEIGHT 450    //复合组件弹出子窗口固定高度
#define POP_SIMULATE_DIALOG_WIDTH 550       //推演条件编辑框宽高
#define POP_SIMULATE_DIALOG_HEIGHT 700

#define COMBOX_START_FLAG "[QUOTE"   //属性编辑值下拉框开始标识
#define COMBOX_LOOP_QUOTE "[QUOTE]"  //循环属性编辑时，引用前面输出的变量

#define COMBOX_STYLESHEET "QComboBox QAbstractItemView::item{height:23px;}\
                            QComboBox QAbstractItemView::item:hover{background-color:#567dbc;}"

#include <QString>

#if defined(Q_CC_MSVC)
#include "typeinfo.h"
#elif defined(Q_CC_GNU)
#include "typeinfo"
#endif

#define TYPE_ID(a) typeid(a).name()
#define MY_ASSERT(a) {if(!a) return;}
#define MY_ASSERT_RETURN_NULL(a) {if(!a) return NULL;}
#define MY_ASSERT_GT_ZERO(a) {if(a <=0 ) return;}

#define MY_BUILD_MODEL_ONLY {if(GlobalWindowState != WINDOW_BUILD_MODEL) return;}
#define MY_SIMULATE_MODEL_ONLY {if(GlobalWindowState != WINDOW_SIMULATE) return;}

#include <QVariant>

typedef  QList<QVariant> VariantList;
typedef  QList<VariantList> DataList;

class MyItem;
class QGraphicsItem;

#include <QBrush>
#include <QPen>
#include <QFont>
#include <QDataStream>
#include <QUuid>
#include <QDateTime>

#include "./item/ItemHeader.h"

//窗口工作区间类型
enum WindowState
{
    WINDOW_BUILD_MODEL,          //模型建立
    WINDOW_SIMULATE              //模型推演
};

//返回值类型
enum ReturnType
{
    RETURN_ERROR = 0,                //错误
    RETURN_SUCCESS = 1,              //正确
    SCENE_NOT_EXIST = 2,             //场景不存在
    SCENE_EMPTY = 3,                 //场景为空
    SCENE_WITHOUT_START = 4,         //场景无开始
    SCENE_MORE_START = 5,            //场景多个开始
    SCENE_WITHOUT_END = 6,           //场景无结束
    SCENE_MORE_END = 7,              //场景多个结束
    SCENE_FLOW_ERROR = 8,            //场景流程错误

    FLOW_NOT_COMPLETE = 10,          //流程不完整
    FLOW_COMPLETE = 12,              //流程完整
    FLOW_SUCCESS = 13,               //流程逻辑正确
    FLOW_ERROR = 14,                 //流程逻辑错误

    FILE_NOT_EXIST = 30,             //文件不存在
    FILE_CANT_READ = 31,             //文件不可读
    FILE_CANT_WRITE = 32,            //文件不可写
    FILE_ILLEGAL = 33,               //文件格式不合法
    FILE_VERSION = 34                //文件的版本不一致(文件与程序版本不一致)

};

/****************************************建模**************************************************/

//当前添加的类型
enum GraphicsType
{
    GRA_NONE,
    GRA_SQUARE,          //正方形
    GRA_RECT,            //矩形
    GRA_ROUND_RECT,      //圆角矩形
    GRA_CIRCLE,          //圆
    GRA_ELLIPSE,         //椭圆
    GRA_POLYGON,         //菱形
    GRA_PARALLELOGRAM,   //平行四边形
    GRA_LOOP_UP,         //循环上限
    GRA_LOOP_DOWN,       //循环下限
    GRA_ANNOTATION,      //注解
    GAR_PARALLE,         //并行
    GRA_LINE,            //直线
    GRA_VECTOR_LINE,     //折线
    GRA_TEXT,            //文本
    GRA_NODE_PORT,       //端口
    GRA_DRAG_POINT,      //控件拖拽点
    GRA_DRAG_LING,       //控件线段拖拽点
    GRA_ROTATE_POINT,    //控件旋转点
    GRA_ITEM_INFO,       //控件显示信息
    //端口
    GRA_NODE_PROCESS,         //原子处理窗口
    GRA_NODE_HALF_CIRCLE,     //D端口
    GRA_NODE_TRIANGLE_IN ,    //三角输入端口
    GRA_NODE_TRIANGLE_OUT,    //三角输出端口
    GRA_NODE_CIRCLE,          //圆输出端口
#ifdef ADD_STATE_MODEL
    //状态机
    GRA_STATE_START,          //状态机开始
    GRA_STATE_END,            //状态机结束
    GRA_STATE_PROCESS,        //状态机处理
    //遮罩
    GRA_MASK_RECT,            //矩形遮罩
    GRA_MASK_BOUND_RECT,      //圆角矩形遮罩
    GRA_MASK_CIRCLE,          //圆形遮罩
#endif
    GRA_NO_MEAN               //无意义值
};

//单元类型
enum ProcessType
{
    PRO_NONE,               //无
    PRO_START,              //开始:圆角矩形
    PRO_INPUT,              //输入输出:平行四边形
    PRO_PROCESS,            //处理:矩形
    PRO_JUDGE,              //判断:菱形
    PRO_PARALLEL,           //并行:平行线
    PRO_LOOP,               //循环
    PRO_END                 //结束:圆角矩形
};

//拖入端口的方向
enum DragDirect
{
    DRAG_NONE,
    DRAG_LEFT,
    DRAG_TOP,
    DRAG_RIGHT,
    DRAG_BOTTOM
};

//端口到达两端后，用于区分到达哪一端
enum DragRange
{
    DRAG_RANG_NONE,
    DRAG_RANG_MAX,
    DRAG_RANG_MIN
};

//分割栏与控件的位置关系
enum SpiltDirect
{
    SPLIT_NONE,
    SPLIT_LEFT,
    SPLIT_TOP,
    SPLIT_RIGHT,
    SPLIT_BOTTOM
};

//锁定的状态
enum ItemLockState
{
    ITEM_ALL_LOCK,             //全部锁定
    ITEM_NOT_ALL_LOCK,         //部分锁定/部分未锁定
    ITEM_ALL_UNLOCK            //全部未锁定
};

//线条的类型
enum AddLineType
{
    LINE_HORIZONTAL,           //直线
    LINE_ARROW,                //箭头
    LINE_SOLID_TRIANGLE        //实心三角箭头
};

//文本信息存在的方式
enum TextExistType
{
    TEXT_ALONG,                //单独存在
    TEXT_CHILD                 //作为某个控件子控件依附存在
};

//记录当前item在scene中的x、y、w、h值
struct MyRect
{
    MyRect()
    {
        x = 0;
        y = 0;
        width = 0;
        height = 0;
    }

    friend QDataStream& operator <<(QDataStream &,MyRect & rect);
    friend QDataStream& operator >>(QDataStream &,MyRect & rect);

    qreal x;
    qreal y;
    qreal width;
    qreal height;
};

//每个item拥有的属性，笔刷、画笔、位置、大小、角度、字体
struct ItemProperty
{
    ItemProperty()
    {
        isNeedBrush = true;
        isNeedBorder = true;
        isFont = false;
        isMoveable = true;
        rotateDegree = 0;
        itemPen.setWidth(1);
        alphaValue = 100;
        zValue = 0;
        startLineType = LINE_HORIZONTAL;
        endLineType = LINE_HORIZONTAL;
        ptype = PRO_NONE;
        createUUID();
    }

    //产生一个新的身份唯一标识，在剪切、复制、本地打开时需要手动的更新此值
    void createUUID()
    {
        startItemID = QUuid::createUuid().toString();    //新建对象时创建唯一标识符
        associativeID = QUuid::createUuid().toString();
    }

    QString startItemID;                  //在用于非直线的控件时，只用startItemId标识当前控件
    QString endItemID;                    //在用于直线控件时，两个表示直线两端连接的控件的ID号
#ifdef ADD_STATE_MODEL
    QString associativeID;                //用于复合组件寻找原子图，原子图寻找状态图的标识,作为文件名【!!!】
#endif

    friend QDataStream & operator <<(QDataStream &,ItemProperty & item);
    friend QDataStream & operator >>(QDataStream &,ItemProperty & item);

    bool isNeedBrush;               //为ture时表示需要笔刷填充
    QBrush itemBrush;
    bool isNeedBorder;              //为true时表示是否需要边框
    QPen   itemPen;

    MyRect itemRect;
    int    alphaValue;              //透明度值
    int    rotateDegree;

    bool isFont;                    //是否为字体
    QString content;                //文字内容
    QFont  itemFont;
    QColor fontColor;               //字体颜色

    qreal zValue;                   //深度值

    LineType lineType;              //线条的类型
    AddLineType startLineType;      //起始线条类型
    AddLineType endLineType;        //终止线条类型
    PointType startPointType;       //线条在【LINE_MYITEM】模式下，起点所在item边的位置
    PointType endPointType;         //线条在【LINE_MYITEM】模式下，终点点所在item边的位置

    bool isMoveable;                //是否被锁定

    ProcessType ptype;              //推演模式代表的类型
};

//节点属性
struct NodePortProperty
{
    NodePortProperty()
    {
        startItemID = QUuid::createUuid().toString();    //新建对象时创建唯一标识符
    }

    friend QDataStream & operator <<(QDataStream &,NodePortProperty & item);
    friend QDataStream & operator >>(QDataStream &,NodePortProperty & item);

    GraphicsType portType;      //节点类型
    QString content;            //节点文字信息
    QBrush itemBrush;           //填充颜色
    DragDirect direct;          //保存的方向
    qreal scaleFactor;          //拖入的位置相当于当前一边所在的比例
    QString parentItemID;       //父控件的ID(解析后寻找停靠点)
    QString startItemID;        //在用于非直线的控件时，只用startItemId标识当前控件(用于箭头寻找父节点)
};

/****************************************属性编辑**************************************************/
//输入参数的属性
struct Parameter
{

    friend QDataStream& operator <<(QDataStream &,Parameter * rect);
    friend QDataStream& operator >>(QDataStream &,Parameter * rect);

    QString pId;               //主键值
    QString pName;             //参数名
    QString pType;             //参数类型
    QString pValue;            //参数值
    QString pRemark;           //备注
};

typedef  QList<Parameter *> ParaList;

//服务的属性
struct ServiceProperty
{
    ServiceProperty()
    {
        hasSettInfo = false;
    }

    friend QDataStream & operator <<(QDataStream &,ServiceProperty * item);
    friend QDataStream & operator >>(QDataStream &,ServiceProperty * item);

    bool hasSettInfo;                 //是否已经设置了信息，用于在显示属性编辑时作为标识是否要从数据库读取
    QString id;                       //数据库主键值
    QString serviceName;              //服务名
    QString status;                   //服务状态
    QString descirption;              //服务描述
    QString servicePath;              //服务路径
    QString method;                   //调用接口
    ParaList inputParas;              //输入参数集合
    ParaList outputParas;             //输出参数集合
    bool isAsNextInput;               //输出是否作为下个服务的输入
};

//服务的基本信息
struct ServiceInfo
{
    QString id;           //服务索引
    QString code;
    QString name;
    QString descirption;
    QString status;
    QString auditOpinion;
    QString method;
    QString input;
    QString result;
    int num;
    QDateTime addTime;
    QString addUser;
    QString auditUser;     //审批人
    QString auditUserName; //审批人姓名
    QString outputName;
    QString outputType;
    QString addUserName;
    QDateTime auditTime;
};

//服务的参数信息
struct ServiceParameter
{
    QString id;
    QString relationId;
    QString parameterName;
    QString parameterType;
    QDateTime addTime;
};

//服务的部署信息
struct ServiceDeploy
{
    QString id;
    QString relationId;
    QString hostIP;
    QString hostId;
    QDateTime addTime;
    QString exampleName;
    QString url;
    QString status;
};

typedef QList<ServiceInfo> ServiceInfoList;

enum LoopPart
{
    LOOP_VARI,          //循环变量定义
    LOOP_EXPRESS,       //循环表达式
    LOOP_FINAL          //循环末尾
};

//变量定义int i = 0;
struct VariableDefine
{
    friend QDataStream & operator <<(QDataStream & stream,VariableDefine *);
    friend QDataStream & operator >>(QDataStream & stream,VariableDefine *);

    QString type;              //int
    QString name;              //i
    QString value;             //0
    QString pRemark;           //备注
};

typedef QList<VariableDefine *> VariableDefineList;

//表达式定义i < 5
struct ExpressDefine
{
    friend QDataStream & operator <<(QDataStream & stream,ExpressDefine *);
    friend QDataStream & operator >>(QDataStream & stream,ExpressDefine *);

    QString name;              //i
    QString expressType;       //<
    QString value;             //5
    QString pRemark;           //备注
};

typedef QList<ExpressDefine *> ExpressDefineList;

//末循环体定义i++/i+=4
struct FinalExpressDefine
{
    friend QDataStream & operator <<(QDataStream & stream,FinalExpressDefine *);
    friend QDataStream & operator >>(QDataStream & stream,FinalExpressDefine *);

    QString name;             //i
    QString expressType;      //++
    QString value;            //4
    QString pRemark;          //备注
};

typedef QList<FinalExpressDefine *> FinalExpressDefineList;

//单个条件的映射关系int i = 0;i<5;i++
struct SignalVari
{
    SignalVari()
    {
       variName = "";
       isLegal = false;
       isQuoted = false;
       isAssignedValue = false;
       initialValue = 0;
       finalValue = 0;
       middlValue = 0;
       operateSymbol = "";
       selfOperateSymbol = "";
       selfOperateValue = 0;
    }

    friend QDataStream & operator <<(QDataStream & stream,SignalVari *);
    friend QDataStream & operator >>(QDataStream & stream,SignalVari *);

    QString variName;           //变量名i
    bool isQuoted;              //是否为引用值,若为引用值那么initialValue和middlValue值需要在运算过程中通过获取前面的服务的输出进行初始化
    bool isAssignedValue;       //在isQuoted为true时，此值有效。如果为false，则需要获取引用的值之后置为true。
    bool isLegal;               //是否合法，如果有某个变量值不存在那么将其置为false，在使用时如果为false那么就不使用
    int initialValue;           //初始值0
    int finalValue;             //终止条件值5
    int middlValue;             //中间值
    QString operateSymbol;      //操作条件符号<、>、<=...
    QString selfOperateSymbol;  //自增++、--、+=
    int selfOperateValue;       //自增值(+=2，表示2)
    QList<int> middleResults;   //循环过程中产生的中间值<key:第几次循环；value：当前循环状态下变量的值>
};

typedef QList<SignalVari *> SignalVariList;

#include <QDebug>

//循环参数属性
struct LoopProperty
{
    friend QDataStream & operator <<(QDataStream & stream,LoopProperty *);
    friend QDataStream & operator >>(QDataStream & stream,LoopProperty *);

    ~LoopProperty()
    {
        foreach(SignalVari * tmp,signalList)
        {
            delete tmp;
        }
        signalList.clear();

        foreach(VariableDefine * tmp,varList)
        {
            delete tmp;
        }
        varList.clear();

        foreach(ExpressDefine * tmp,expList)
        {
            delete tmp;
        }
        expList.clear();

        foreach(FinalExpressDefine * tmp,fexpList)
        {
            delete tmp;
        }
        fexpList.clear();
    }

    SignalVariList signalList;          //多个条件集合
    VariableDefineList varList;         //变量集合
    ExpressDefineList expList;          //表达式集合
    FinalExpressDefineList fexpList;    //末循环体集合
};

//判断框属性
struct JudgeProperty
{
    friend QDataStream & operator <<(QDataStream &,JudgeProperty *prop);
    friend QDataStream & operator >>(QDataStream &,JudgeProperty * prop);

    QString express;            //表达式
    QString switchExpress;      //如果引用，则保存将引用转换成对应参数后的表达式
};

//数据显示表的行号和数据在数据库中index对应关系
struct RowToIndex
{
    int rowNum;
    QString indexNum;
};

typedef QList< RowToIndex > RowList;

//暂存一个剪切时的控件信息
struct CutInfo
{
    CutInfo()
    {
        hasContent = false;
        loopProp = new LoopProperty;
        judgeProp = new JudgeProperty;
    }

    bool  hasContent;                            //是否包拷贝内容,在剪切和复制后置为true，清空后置为false
    GraphicsType graphicsType;
    ItemProperty itemProperty;
    ServiceProperty serviceProp;                 //服务属性信息
    LoopProperty * loopProp;                     //服务循环属性信息
    JudgeProperty * judgeProp;                   //服务判断属性信息
    QString content;
    QList<NodePortProperty> nodeProperties;      //控件包含端口的信息
};

//高亮等级
enum HightLightLevel
{
    LEVEL_NORMAL,        //普通(黑)
    LEVEL_MIDDLE,        //中级(红)推演流程
    LEVEL_HIGH           //高级(蓝)推演流程选中
};

/*************************循环结构体各部分参数定义******************************/
//处理单元
struct ProcessUnit
{
    ProcessUnit()
    {
        item = NULL;
        nextChild = NULL;
        yesChild = NULL;
        noChild = NULL;
    }
    ProcessType ptype;               //处理单元类型
    GraphicsType gtype;              //控件的类型

    MyItem * item;                   //处理单元对应的控件Item
    ProcessUnit * nextChild;         //非判断框下，指向后面索引
    ProcessUnit * yesChild;          //判断框状态下，yes和no child启用
    ProcessUnit * noChild;           //
};

#ifdef ADD_STATE_MODEL
/**************************状态机结构体******************************/
//初始化端口设置
struct StatePortProperty
{
    QString portName;        //端口名称
    QString portType;        //端口类型
};

//输入/出端口设置
struct StateInOutProperty
{
    QString portName;        //端口名称
    QString portType;        //端口类型
    QList<StatePortProperty> props;    //端口集合
};

//开始状态属性
struct StateStartProperty
{
    QString content;
};

//内部事件属性
struct StatInnerProperty
{
    QString propName;
    QString propType;
    QString propDesc;
};

//模型状态属性
struct StateModelProperty
{
    QString stateName;          //状态名
    QString continueContent;    //持续事件行为
    QList<StatInnerProperty> props;   //内部事件集合
};

#endif

#endif // HEADER_H

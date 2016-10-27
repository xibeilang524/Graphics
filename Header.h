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

#define M_VERTION 0x0005        //程序的版本，在保存文件时，要保存当前文件的版本；解析时也要判断

#define PI 3.141592653
#define ICON_WIDTH  60          //拖拽图标的宽高
#define ICON_HEIGHT 60
#define LAYOUT_SPACING  1       //布局管理器中两控件间的距离
#define FONT_DEAFULT_PIX  9     //字体初始化大小
#define ROTATE_MIN_DEGREE 0     //旋转的最小角度
#define ROTATE_MAX_DEGREE 360   //旋转的最大角度

#include <QString>

#if defined(Q_CC_MSVC)
#include "typeinfo.h"
#elif defined(Q_CC_GNU)
#include "typeinfo"
#endif

#define TYPE_ID(a) typeid(a).name()
#define MY_ASSERT(a) {if(!a) return;}
#define MY_ASSERT_GT_ZERO(a) {if(a <=0 ) return;}

#define MY_BUILD_MODEL_ONLY {if(GlobalWindowState != WINDOW_BUILD_MODEL) return;}
#define MY_SIMULATE_MODEL_ONLY {if(GlobalWindowState != WINDOW_SIMULATE) return;}

#include <QVariant>

typedef  QList<QVariant> VariantList;
typedef  QList<VariantList> DataList;

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
    GRA_NODE_HALF_CIRCLE,  //D端口
    GRA_NODE_TRIANGLE_IN ,    //三角输入端口
    GRA_NODE_TRIANGLE_OUT,    //三角输出端口
    GRA_NODE_CIRCLE           //圆输出端口
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
        createUUID();
    }

    //产生一个新的身份唯一标识，在剪切、复制、本地打开时需要手动的更新此值
    void createUUID()
    {
        startItemID = QUuid::createUuid().toString();    //新建对象时创建唯一标识符
    }

    QString startItemID;                  //在用于非直线的控件时，只用startItemId标识当前控件
    QString endItemID;                    //在用于直线控件时，两个表示直线两端连接的控件的ID号

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
    QBrush itemBrush;           //填充颜色
    DragDirect direct;          //保存的方向
    qreal scaleFactor;          //拖入的位置相当于当前一边所在的比例
    QString parentItemID;       //父控件的ID(解析后寻找停靠点)
    QString startItemID;        //在用于非直线的控件时，只用startItemId标识当前控件(用于箭头寻找父节点)
};

//暂存一个剪切时的控件信息
struct CutInfo
{
    CutInfo()
    {
        hasContent = false;
    }
    bool  hasContent;                            //是否包拷贝内容,在剪切和复制后置为true，清空后置为false
    GraphicsType graphicsType;
    ItemProperty itemProperty;
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

/****************************************属性编辑**************************************************/
//输入参数的属性
struct Parameter
{
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
    bool hasSettInfo;                 //是否已经设置了信息，用于在显示属性编辑时作为标识是否要从数据库读取
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

//数据显示表的行号和数据在数据库中index对应关系
struct RowToIndex
{
    int rowNum;
    QString indexNum;
};

typedef QList< RowToIndex > RowList;

#endif // HEADER_H

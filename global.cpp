#include "global.h"

MainWindow * GlobalMainWindow;            //顶层的主窗口

GraphicsType CurrAddGraType;              //当前添加的类型

int SceneWidth,SceneHeight;               //scene的宽度和高度
int ScreenWidth,ScreenHeight;             //屏幕的宽度和高度
qreal GlobalItemZValue;                   //item的纵向深度，用于在拖入控件时自动的增加

QPointF SceneLastClickPoint;              //scene中最后一次鼠标点击的位置，作为粘贴时的位置

QString SaveFileSuffix;                   //保存文件的后缀名
QString SaveFileHeadFlag;                 //报文文件的文件头，用于判断所选择的文件是否是程序支持的格式

QString GlobalLastSQLError;               //数据库访问错误，保留最新的错误

WindowState GlobalWindowState;            //全局的工作区间类型

int GlobalRotateStep; 		 		 	  //旋转到对应角度才进行角度切换
qreal GlobalMinMumSize;            		  //控件缩小的最小范围
qreal GlobalParalMinMumSize;              //平行控件缩小的最小范围

QList<ServiceProperty> GlobalServiceProperties;     //全局的服务信息

#ifndef GLOBAL_H
#define GLOBAL_H

#include "Header.h"

class MainWindow;

extern MainWindow * GlobalMainWindow;            //顶层的主窗口

extern GraphicsType CurrAddGraType;              //当前添加的类型

extern int SceneWidth,SceneHeight;               //scene的宽度和高度
extern int ScreenWidth,ScreenHeight;             //屏幕的宽度和高度
extern qreal GlobalItemZValue;                   //item的纵向深度，用于在拖入控件时自动的增加

extern QPointF SceneLastClickPoint;              //scene中最后一次鼠标点击的位置，作为粘贴时的位置

extern QString SaveFileSuffix;                   //保存文件的后缀名
extern QString SaveFileHeadFlag;                 //报文文件的文件头，用于判断所选择的文件是否是程序支持的格式

extern QString GlobalLastSQLError;               //数据库访问错误，保留最新的错误

extern WindowState GlobalWindowState;            //全局的工作区间类型

extern int GlobalRotateStep; 		 		 	  //旋转到对应角度才进行角度切换
extern qreal GlobalMinMumSize;            		  //控件缩小的最小范围

extern QList<ServiceProperty *> GlobalServiceProperties;     //全局的服务信息
extern QList<ServiceProperty *> PreExeServices;              //预处理服务
extern QList<ServiceProperty *> ResetExeServices;            //重置服务

extern bool GlobalIsSimulateState;               //是否为推演状态，如果是则不允许进行状态变更
extern bool IsConfigIniExisted;                  //全局的配置文件是否存在

extern QMap<QString,QString> GlobalKeyMap;       //快捷键注册信息

#endif // GLOBAL_H

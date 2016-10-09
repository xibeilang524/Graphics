#ifndef GLOBAL_H
#define GLOBAL_H

#include "Header.h"

extern GraphicsType CurrAddGraType;              //当前添加的类型

extern int SceneWidth,SceneHeight;               //scene的宽度和高度
extern int ScreenWidth,ScreenHeight;             //屏幕的宽度和高度
extern qreal GlobalItemZValue;                   //item的纵向深度，用于在拖入控件时自动的增加

extern QPointF SceneLastClickPoint;              //scene中最后一次鼠标点击的位置，作为粘贴时的位置

extern QString SaveFileSuffix;                   //保存文件的后缀名
extern QString SaveFileHeadFlag;                 //报文文件的文件头，用于判断所选择的文件是否是程序支持的格式

#endif // GLOBAL_H

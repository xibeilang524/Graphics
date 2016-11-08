/*************************************************
**版  权：RenGu Company
**文件名: util.h
**作  者: wey       Version: 1.0       Date: 2016.XX.XX
**描  述:
**Others:
**
**修改历史:
**20161018:gx:弧度角度互换radinToAngle
**20161026:wey:调整全局的弹出框
**20161108:wey:添加对状态图/遮罩的支持
*************************************************/
#ifndef UTIL_H
#define UTIL_H

#include <QColor>
#include <QFont>
#include <QPixmap>
#include <QMessageBox>

#include "Header.h"

class QWidget;
class QTableWidget;

class Util
{
public:
    static void setWidgetColor(QWidget * widget,QColor & color);
    static QString getFontInfo(QFont font);
    static qreal getGlobalZValue();
    static void  resetGlobalZValue();
    static QString getUUID();
    static qreal getMaxNum(qreal & a,qreal & b);
    static void setTableStylesheet(QTableWidget * table);

    static void createTableItem(const int rowCount,QTableWidget * table);
    static void removeTableItem(QTableWidget * table);
    static void clearTableData(QTableWidget * table);
    static qreal radinToAngle(const qreal &radin);
    static void loadPixmapByGType(GraphicsType type,QPixmap & pixmap);

    static void showWarn(QString tipText);
    static int getWarnChoice(QString tipText, QMessageBox::StandardButton butt = QMessageBox::No);
    static void showInfo(QString tipText);
    static QMessageBox::StandardButton getInfoChoice(QString tipText,QMessageBox::StandardButton butt = QMessageBox::Yes);
};

#endif // UTIL_H

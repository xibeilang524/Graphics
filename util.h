/*************************************************
**版  权：RenGu Company
**文件名: util.h
**作  者: wey       Version: 1.0       Date: 2016.XX.XX
**描  述:
**Others:
**
**修改历史:
**20161018:gx:弧度角度互换radinToAngle
*************************************************/
#ifndef UTIL_H
#define UTIL_H

#include <QColor>
#include <QFont>
#include <QPixmap>

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
};

#endif // UTIL_H

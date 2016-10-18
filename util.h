#ifndef UTIL_H
#define UTIL_H

#include <QColor>
#include <QFont>

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
};

#endif // UTIL_H

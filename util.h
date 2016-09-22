#ifndef UTIL_H
#define UTIL_H

#include <QColor>
#include <QFont>

class QWidget;

class Util
{
public:
    static void setWidgetColor(QWidget * widget,QColor & color);
    static QString getFontInfo(QFont font);
    static qreal getGlobalZValue();
    static void  resetGlobalZValue();
};

#endif // UTIL_H

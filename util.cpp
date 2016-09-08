#include "util.h"

#include <QWidget>

void Util::setWidgetColor(QWidget *widget, QColor &color)
{
    widget->setStyleSheet("background-color:rgba("+QString::number(color.red())\
                          +","+QString::number(color.green())\
                          +","+QString::number(color.blue())\
                          +","+QString::number(color.alpha())+")");
}

//获取字体的信息
QString Util::getFontInfo(QFont font)
{
    return QString("%1px,%2").arg(font.pointSize()).arg(font.family());
}

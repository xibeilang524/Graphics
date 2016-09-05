#include "util.h"

#include <QWidget>

void Util::setWidgetColor(QWidget *widget, QColor &color)
{
    widget->setStyleSheet("background-color:rgba("+QString::number(color.red())\
                          +","+QString::number(color.green())\
                          +","+QString::number(color.blue())\
                          +","+QString::number(color.alpha())+")");
}

#include "util.h"

#include <QWidget>
#include <QUuid>

#include "global.h"

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

//决定每个控件的纵向深度，在添加或复制时候自动调用
qreal Util::getGlobalZValue()
{
    GlobalItemZValue += 0.1;
    return GlobalItemZValue;
}

//在清空控件时自动清0
void Util::resetGlobalZValue()
{
    GlobalItemZValue = 0;
}

//获取唯一标识符
QString Util::getUUID()
{
    return QUuid::createUuid().toString();
}

qreal Util::getMaxNum(qreal &a, qreal &b)
{
    if(a >= b)
    {
        return a;
    }
    else
    {
        return b;
    }
    return 0;
}

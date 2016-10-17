/*************************************************
**版  权：RenGu Company
**文件名: sceneitempickup.h
**作  者: wey       Version: 1.0       Date: 2016.10.11
**描  述:
**Others:场景控件提取
**      【1】:提取当前场景中控件和线条
 *      【2】:判断是否包含开始和结束
**修改历史:
**20161011:wey:建立初始版本，实现控件选取和起始点验证
*************************************************/
#ifndef SCENEITEMPICKUP_H
#define SCENEITEMPICKUP_H

#include "SimulateHeader.h"
class QGraphicsItem;

class SceneItemPickup
{
public:
    static SceneItemPickup * instance();

    ReturnType pickUpItems(QList<QGraphicsItem *> &existedItems);
    ReturnType validateStartEnd(QList<QGraphicsItem *> &existedItems);

private:
    SceneItemPickup();
    static SceneItemPickup * pickup;
};

#endif // SCENEITEMPICKUP_H

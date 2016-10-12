#include "sceneitempickup.h"

#include "../item/mygraphicsview.h"
#include "../item/myscene.h"
#include "../item/myitem.h"

SceneItemPickup * SceneItemPickup::pickup = NULL;

SceneItemPickup::SceneItemPickup()
{

}

SceneItemPickup * SceneItemPickup::instance()
{
    if(!pickup)
    {
        pickup = new SceneItemPickup();
    }
    return pickup;
}

//提取场景中存在的控件,只要提取MyItem和线段(MyArrow)
ReturnType SceneItemPickup::pickUpItems(QList<QGraphicsItem *> &existedItems)
{
    //场景不可访问
    if(MyGraphicsView::instance()->scene() == NULL)
    {
        return SCENE_NOT_EXIST;
    }

    //场景内容为空
    if(MyGraphicsView::instance()->scene()->items().size() == 0)
    {
        return SCENE_EMPTY;
    }

    QList<QGraphicsItem *> items = MyGraphicsView::instance()->scene()->items();

    //【1】:提取当前场景中控件和线条
    QString typeId;
    foreach(QGraphicsItem * item,items)
    {
        typeId = TYPE_ID(*item);
        if(typeId == TYPE_ID(MyItem) || typeId == TYPE_ID(MyArrow))
        {
            existedItems.push_back(item);
        }
    }

    return validateStartEnd(existedItems);
}

//验证开始和结束是否正确
ReturnType SceneItemPickup::validateStartEnd(QList<QGraphicsItem *> &existedItems)
{
    //【2】:判断是否包含开始和结束
    int startItemCount = 0;
    int endItemCount = 0;

    foreach(QGraphicsItem * item,existedItems)
    {
        if(TYPE_ID(*item) == TYPE_ID(MyItem))
        {
            MyItem * tmpItem = dynamic_cast<MyItem *>(item);
            if(tmpItem && tmpItem->getType() == GRA_ROUND_RECT)
            {
                QString text = tmpItem->getText();
                if(text == "开始" || text.toLower().contains("start"))
                {
                    startItemCount++;
                }
                else if(text == "结束" || text.toLower().contains("end"))
                {
                    endItemCount++;
                }
            }
        }
    }

    if(startItemCount == 0)
    {
        return SCENE_WITHOUT_START;
    }
    else if(startItemCount > 1)
    {
        return SCENE_MORE_START;
    }

    if(endItemCount == 0)
    {
        return SCENE_WITHOUT_END;
    }
    else if(endItemCount > 1)
    {
        return SCENE_MORE_END;
    }

    return RETURN_SUCCESS;
}

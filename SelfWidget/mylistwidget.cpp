#include "mylistwidget.h"

#include <QMimeData>
#include <QDrag>
#include <QMouseEvent>

#include "../manager/listitemmanager.h"
#include "../Header.h"
#include "../util.h"

MyListWidget::MyListWidget(QWidget *parent):
    QListWidget(parent)
{
//     setViewMode(QListView::IconMode);
//     setIconSize(QSize(ICON_WIDTH,ICON_WIDTH));
     setSpacing(4);
     setAcceptDrops(false);
}

void MyListWidget::mousePressEvent(QMouseEvent *event)
{
    startPressPoint = event->pos();
    QListWidget::mousePressEvent(event);
}

void MyListWidget::mouseMoveEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
    MyListItem * item = dynamic_cast<MyListItem*>(itemAt(startPressPoint));

    if(item)
    {
        QByteArray itemData;
        QDataStream dataStream(&itemData, QIODevice::WriteOnly);
        int graphicsType = item->data(Qt::UserRole).toInt();

        dataStream << graphicsType;

        QMimeData *mimeData = new QMimeData;
        mimeData->setData("MyItem", itemData);

        QDrag *drag = new QDrag(this);
        drag->setMimeData(mimeData);

        QPixmap pixmap;

        Util::loadPixmapByGType((GraphicsType)graphicsType,pixmap);

        pixmap = pixmap.scaled(ICON_WIDTH,ICON_HEIGHT);

        drag->setHotSpot(QPoint(pixmap.width()/2, pixmap.height()/2));

        drag->setPixmap(pixmap);
        if (drag->exec(Qt::MoveAction) == Qt::MoveAction)
        {

        }
    }
}

//拖动
void MyListWidget::startDrag(Qt::DropActions supportedActions)
{
    Q_UNUSED(supportedActions)

     MyListItem * item = dynamic_cast<MyListItem*>(currentItem());

     if(item)
     {
         QByteArray itemData;
         QDataStream dataStream(&itemData, QIODevice::WriteOnly);
         int graphicsType = item->data(Qt::UserRole).toInt();

         dataStream << graphicsType;

         QMimeData *mimeData = new QMimeData;
         mimeData->setData("MyItem", itemData);

         QDrag *drag = new QDrag(this);
         drag->setMimeData(mimeData);

         QPixmap pixmap;
         Util::loadPixmapByGType((GraphicsType)graphicsType,pixmap);

         pixmap = pixmap.scaled(ICON_WIDTH,ICON_HEIGHT);

         drag->setHotSpot(QPoint(pixmap.width()/2, pixmap.height()/2));

         drag->setPixmap(pixmap);
         if (drag->exec(Qt::MoveAction) == Qt::MoveAction)
         {

         }
     }
//          delete takeItem(row(item));    //删除选中的item
}

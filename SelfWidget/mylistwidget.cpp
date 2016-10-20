#include "mylistwidget.h"

#include <QMimeData>
#include <QDrag>
#include <QMouseEvent>
#include <QDebug>

#include "../manager/listitemmanager.h"
#include "../Header.h"

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
        switch(graphicsType)
        {
            case GRA_SQUARE:
                               pixmap.load(":/images/square.png");
                               break;
            case GRA_RECT:
                               pixmap.load(":/images/rectange.png");
                               break;
            case GRA_ROUND_RECT:
                               pixmap.load(":/images/roundedrect.png");
                               break;
            case GRA_CIRCLE:
                               pixmap.load(":/images/circle.png");
                               break;
            case GRA_ELLIPSE:
                               pixmap.load(":/images/ellipse.png");
                               break;
            case GRA_POLYGON:
                               pixmap.load(":/images/diamonds.png");
                               break;
            case GRA_TEXT:
                               pixmap.load(":/images/text.png");
                               break;
            case GRA_NODE_PORT:
                               pixmap.load(":/images/nodePort.png");
                               break;
            case GRA_PARALLELOGRAM:
                               pixmap.load(":/images/parallelogram.png");
                               break;
            case GRA_LOOP_UP:
                               pixmap.load(":/images/loop_up.png");
                               break;
            case GRA_LOOP_DOWN:
                               pixmap.load(":/images/loop_down.png");
                               break;
            case GRA_ANNOTATION:
                               pixmap.load(":/images/annotation.png");
                               break;
            case GAR_PARALLE:
                               pixmap.load(":/images/parallel.png");
                               break;
            default:
                               break;
        }

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
         switch(graphicsType)
         {
            case GRA_SQUARE:
                                pixmap.load(":/images/square.png");
                                break;
             case GRA_RECT:
                                pixmap.load(":/images/rectange.png");
                                break;
             case GRA_ROUND_RECT:
                                pixmap.load(":/images/roundedrect.png");
                                break;
             case GRA_CIRCLE:
                                pixmap.load(":/images/circle.png");
                                break;
             case GRA_ELLIPSE:
                                pixmap.load(":/images/ellipse.png");
                                break;
             case GRA_POLYGON:
                                pixmap.load(":/images/diamonds.png");
                                break;
             case GRA_TEXT:
                                pixmap.load(":/images/text.png");
                                break;
             case GRA_NODE_PORT:
                                pixmap.load(":/images/nodePort.png");
                                break;
             default:
                                break;
         }

         pixmap = pixmap.scaled(ICON_WIDTH,ICON_HEIGHT);

         drag->setHotSpot(QPoint(pixmap.width()/2, pixmap.height()/2));

         drag->setPixmap(pixmap);
         if (drag->exec(Qt::MoveAction) == Qt::MoveAction)
         {

         }
     }
//          delete takeItem(row(item));    //删除选中的item
}

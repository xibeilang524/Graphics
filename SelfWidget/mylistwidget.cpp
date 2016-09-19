#include "mylistwidget.h"

#include <QMimeData>
#include <QDrag>
#include <QDebug>

#include "listitemmanager.h"
#include "../Header.h"

MyListWidget::MyListWidget(QWidget *parent):
    QListWidget(parent)
{
     setViewMode(QListView::IconMode);
     setIconSize(QSize(50,50));
     setSpacing(10);
     setAcceptDrops(false);
}

//ÍÏ¶¯
void MyListWidget::startDrag(Qt::DropActions supportedActions)
{
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
             case GRA_LINE:
                                pixmap.load(":/images/linepointer.png");
                                break;
             case GRA_VECTOR_LINE:
                                pixmap.load(":/images/vectorLine.png");
                                break;
             case GRA_TEXT:
                                pixmap.load(":/images/text.png");
                                break;
             case GRA_NODE_PORT:
                                pixmap.load(":/images/nodePort.png");
                                break;
         }

         pixmap = pixmap.scaled(ICON_WIDTH,ICON_HEIGHT);

         drag->setHotSpot(QPoint(pixmap.width()/2, pixmap.height()/2));

         drag->setPixmap(pixmap);
         if (drag->exec(Qt::MoveAction) == Qt::MoveAction)
         {

         }
     }
//          delete takeItem(row(item));
}

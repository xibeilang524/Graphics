#include "mylistwidget.h"

#include <QMimeData>
#include <QDrag>

MyListWidget::MyListWidget(QWidget *parent):
    QListWidget(parent)
{
    setViewMode(QListView::IconMode);
    setIconSize(QSize(60,60));
    setSpacing(10);
    setAcceptDrops(false);
}

void MyListWidget::startDrag(Qt::DropActions supportedActions)
{
//     QListWidgetItem *item = currentItem();

     QListWidgetItem *item = currentItem();

      QByteArray itemData;
      QDataStream dataStream(&itemData, QIODevice::WriteOnly);
      QPixmap pixmap = qvariant_cast<QPixmap>(item->data(Qt::UserRole));
      QPoint location = item->data(Qt::UserRole+1).toPoint();

      dataStream << pixmap << location;

      QMimeData *mimeData = new QMimeData;
      mimeData->setData("MyItem", itemData);

      QDrag *drag = new QDrag(this);
      drag->setMimeData(mimeData);
      drag->setHotSpot(QPoint(pixmap.width()/2, pixmap.height()/2));
      drag->setPixmap(pixmap);

      if (drag->exec(Qt::MoveAction) == Qt::MoveAction)
          delete takeItem(row(item));

//     QByteArray itemData;
//     QDataStream dataStream(&itemData, QIODevice::WriteOnly);
//     QPixmap pixmap = qvariant_cast<QPixmap>(item->data(Qt::UserRole));
//     QPoint location = item->data(Qt::UserRole+1).toPoint();

//     dataStream << pixmap << location;

//     QMimeData *mimeData = new QMimeData;
//     mimeData->setData("image/x-puzzle-piece", itemData);

//     QDrag *drag = new QDrag(this);
//     drag->setMimeData(mimeData);
//     drag->setHotSpot(QPoint(pixmap.width()/2, pixmap.height()/2));
//     drag->setPixmap(pixmap);

//     if (drag->exec(Qt::MoveAction) == Qt::MoveAction)
//         delete takeItem(row(item));
}

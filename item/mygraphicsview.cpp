#include "mygraphicsview.h"

#include <QDropEvent>
#include <QMimeData>
#include <QDataStream>
#include <QDebug>

#include "myscene.h"
#include "../SelfWidget/nodeeditdialog.h"
#include "../Header.h"
#include "../global.h"

MyGraphicsView * MyGraphicsView::viewManager = NULL;

MyGraphicsView * MyGraphicsView::instance()
{
   return viewManager;
}

MyGraphicsView::MyGraphicsView(QWidget * parent):
    QGraphicsView(parent)
{
    viewManager = this;

    nodeEdit = NULL;

    setAcceptDrops(true);

}

//左侧控件拖入，对拖入的类型进行判断
void MyGraphicsView::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData()->hasFormat("MyItem"))
    {
        QByteArray array = event->mimeData()->data("MyItem");
        QDataStream stream(&array, QIODevice::ReadOnly);//流，读数据

        int graphicsType;
        stream>>graphicsType;
        if(graphicsType == GRA_LINE || graphicsType == GRA_VECTOR_LINE || graphicsType == GRA_NODE_PORT)
        {
            event->ignore();
        }
        else
        {
           event->acceptProposedAction();
        }
    }
    else
    {
        event->ignore();
    }
}

void MyGraphicsView::dragMoveEvent(QDragMoveEvent *event)
{
    if(event->mimeData()->hasFormat("MyItem"))
    {
       event->acceptProposedAction();
    }
}

void MyGraphicsView::dropEvent(QDropEvent *event)
{
    if(event->mimeData()->hasFormat("MyItem"))
    {
        QByteArray array = event->mimeData()->data("MyItem");
        QDataStream stream(&array, QIODevice::ReadOnly);//流，读数据

        int graphicsType;
        stream>>graphicsType;
        if(graphicsType == GRA_LINE || graphicsType == GRA_VECTOR_LINE || graphicsType == GRA_NODE_PORT)
        {

        }
        else
        {
           MyScene * tmpScene = dynamic_cast<MyScene *>(scene());
           if(tmpScene)
           {
               tmpScene->addItem((GraphicsType)graphicsType,mapToScene(event->pos()));
           }
        }
    }
}

//显示端口编辑窗口
void MyGraphicsView::showNodePortEdit(MyNodePort *nodePort)
{
    if(!nodeEdit)
    {
        nodeEdit = new NodeEditDialog(this);
    }
    nodeEdit->initNodeInfo(nodePort);
    nodeEdit->exec();
}

MyGraphicsView::~MyGraphicsView()
{
    if(nodeEdit)
    {
        delete nodeEdit;
        nodeEdit = NULL;
    }
}

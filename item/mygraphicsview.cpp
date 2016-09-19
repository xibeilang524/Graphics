#include "mygraphicsview.h"

#include "../SelfWidget/nodeeditdialog.h"

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
}

//ÏÔÊ¾¶Ë¿Ú±à¼­´°¿Ú
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

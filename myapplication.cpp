#include "myapplication.h"

#include <QEvent>
#include <QKeyEvent>
#include <QDebug>

#include "global.h"
#include "mainwindow.h"

MyApplication::MyApplication(int &argc, char **argv):
    QApplication(argc,argv)
{

}

bool MyApplication::notify(QObject * obj, QEvent * event)
{
    if(event->type() == QEvent::KeyPress)
    {
        QKeyEvent * keyEvent = dynamic_cast<QKeyEvent *>(event);
        if(keyEvent->modifiers() == Qt::AltModifier && (keyEvent->key() == Qt::Key_Left || keyEvent->key() == Qt::Key_Right))
        {
            GlobalMainWindow->keyPress(keyEvent);
        }
    }

    return QApplication::notify(obj,event);
}

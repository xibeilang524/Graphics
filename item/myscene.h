#ifndef MYSCENE_H
#define MYSCENE_H

#include <QGraphicsScene>

#include "../Header.h"

class QMenu;

class MyScene : public QGraphicsScene
{
    Q_OBJECT
public:
    MyScene(QMenu * menu,QObject * parent = 0 );
    ~MyScene();

signals:
    void resetItemAction();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

private:


    QMenu * rightMenu;

};

#endif // MYSCENE_H

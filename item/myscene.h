#ifndef MYSCENE_H
#define MYSCENE_H

#include <QGraphicsScene>

#include "../Header.h"

class QMenu;
class MyArrow;

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
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    QMenu * rightMenu;

    QGraphicsLineItem * insertTmpLine;

};

#endif // MYSCENE_H

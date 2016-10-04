#ifndef MYPAGEITEM_H
#define MYPAGEITEM_H

#include <QWidget>

class QLabel;
class QPushButton;

class MyPageItem : public QWidget
{
    Q_OBJECT
public:
    explicit MyPageItem(QWidget *parent = 0);
    void setText(QString text);
    void setSelected(bool);
    bool getSelected(){return this->isSelected;}

    void setId(QString id){this->id = id;}
    QString getId(){return this->id;}

signals:
    void switchPage(QString);
    void deletePage(QString);

private slots:
    void deleteMe();

protected:
    void mousePressEvent(QMouseEvent *);
    
private:
    void initWidget();

    QLabel * iconLabel;
    QLabel * textLabel;
    QPushButton * exitButt;

    bool isSelected;           //是否被选中
    QString id;
    
};

class PageManager
{
public:
    static PageManager * instance();
    MyPageItem * addPageItem();
    int getPageCount(){return ++pageCount;}
    void resetPageCount(){pageCount = 0;}

private:
    PageManager();
    static PageManager * manager;
    int pageCount;
};

#endif // MYPAGEITEM_H

/*************************************************
**版  权：RenGu Company
**文件名: mypageswitch.h
**作  者: wey       Version: 1.0       Date: 2016.10.03
**描  述:
**Others: 页面切换
**
**修改历史:
**20161004:wey:修复删除工作区奔溃问题
**20161017: gx:调整新建工作区后默认插入到最后
**20161015:wey:增加Alt+左右键快速切换工作区间
**20161025:wey:增加工作区间右键菜单，支持关闭、保存、关闭左侧所有、关闭右侧所有
**20161026:wey:修复工作区间个数为0时，依然可以响应Ctrl+w的事件
*************************************************/
#ifndef MYPAGESWITCH_H
#define MYPAGESWITCH_H

#include <QWidget>

class QPushButton;
class QHBoxLayout;
class MyScene;
class MyPageItem;
class QAction;

class MyPageSwitch : public QWidget
{
    Q_OBJECT
public:
    static MyPageSwitch * instance();

    void switchFrontBack(bool isFront = true);
    void addSwitchContextMenu();

    struct PageMapping
    {
        QString id;
        MyPageItem * pageItem;
        MyScene * scene;
        QString pageName;
        qreal hScrollValue;
        qreal vScrollValue;
        qreal scaleView;
    };

signals:
    void deletePage();
    void switchPage();

public slots:
    void addPage();
    void closePage();

private slots:
    void switchToPage(QString pageId);
    void deleteThisPage(QString pageId);
    void closeLeftPage();
    void closeRightPage();

private:
    MyPageSwitch(QWidget * parent = 0);
    static MyPageSwitch * pageSwitch;

    void siwtchPage(QString pageId,bool firstView = false);
    int getPageIndex(QString id);

    void initWidget();

    QPushButton * addPageButt;
    QHBoxLayout * layout;

    QList<PageMapping *> pages;
    int pagePosition;                     //页面位置
    PageMapping * selectedPage;
    bool isFirstView;
};

#endif // MYPAGESWITCH_H

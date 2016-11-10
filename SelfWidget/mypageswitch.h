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

struct PageMapping
{
    PageMapping()
    {
        isAssociated = false;
    }
    QString id;
    MyPageItem * pageItem;
    MyScene * scene;
    QString pageName;
    QString pathName;        //路径名(文件夹名)
    QString fullPathName;    //(如果保存)当前场景文件显示图元的全路径名称
    bool isAssociated;       //是否被关联，如果false，保存时需要弹出提示框，否则根据id和路径自动保存。
    qreal hScrollValue;
    qreal vScrollValue;
    qreal scaleView;
};

class MyPageSwitch : public QWidget
{
    Q_OBJECT
public:
    static MyPageSwitch * instance();

    void switchFrontBack(bool isFront = true);
    void addSwitchContextMenu();

    bool openPage(QString pageId);
    void updateCurrMappingName(QString name);
    void updateCurrMappingPathName(QString name);

    const PageMapping * currPageMapping();

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
    void addPage(QString id, bool isAssociated = false);
    void initWidget();

    QPushButton * addPageButt;
    QHBoxLayout * layout;

    QList<PageMapping *> pages;
    int pagePosition;                     //页面位置
    PageMapping * selectedPage;
    bool isFirstView;
};

#endif // MYPAGESWITCH_H

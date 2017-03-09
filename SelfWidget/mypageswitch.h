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
**20161201:wey:增加根据文件名全路径切换
**20170309:wey:PageMapping新增linkedItem用于引用遮罩页面的双击控件
*************************************************/
#ifndef MYPAGESWITCH_H
#define MYPAGESWITCH_H

#include <QWidget>

#include "Header.h"

class QPushButton;
class QHBoxLayout;
class MyScene;
class MyPageItem;
class QAction;
class MyItem;

//建立文件与页面、场景之间的关系，此实例可以描述当前页面的基本信息
struct PageMapping
{
    PageMapping()
    {
        isAssociated = false;
#ifdef  ADD_STATE_MODEL
        linkedItem = NULL;
#endif
    }
    QString id;
    MyPageItem * pageItem;
    MyScene * scene;         //关联的scene
#ifdef  ADD_STATE_MODEL
    MyItem * linkedItem;     //双击某个遮罩，将此遮罩控件引用传递至当前页面，以新页面中引用
#endif
    QString pageName;        //工作区名
    QString pathName;        //路径名(文件夹名):c:/abc
    QString fullPathName;    //(如果保存)当前场景文件显示图元的全路径名称:c:/abc/123.bin
    bool isAssociated;       //是否被关联，如果false，保存时需要弹出提示框，否则根据id和路径自动保存。
    qreal hScrollValue;      //当前页面水平滚动条值
    qreal vScrollValue;      //当前页面竖直滚动条值
    qreal scaleView;         //当前页面缩放比例值
};

class MyPageSwitch : public QWidget
{
    Q_OBJECT
public:
    static MyPageSwitch * instance();

    void switchFrontBack(bool isFront = true);
    void addSwitchContextMenu();

    bool openPage(QString pageId);
    void updateCurrMappingInfo(QString & fileName, MyItem *linkItem = NULL);

    void switchToPageByFileName(QString fileName);

    void emitSwitchPage();

    const PageMapping * currPageMapping();

    QStringList getOpenedFullFileNames();
    bool hasContainFile(QString & fileName);

    int  count()const;

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

#include "mypageswitch.h"

#include <QHBoxLayout>
#include <QPushButton>
#include <QList>
#include <QMessageBox>
#include <QDebug>
#include <QFileInfo>
#include <QMenu>

#include "../manager/mypageitem.h"
#include "../manager/actionmanager.h"
#include "../manager/menumanager.h"
#include "../util.h"
#include "../item/myscene.h"
#include "../item/mygraphicsview.h"
#include "../global.h"
#include "../Constants.h"
#include "../mainwindow.h"
#include "Header.h"

using namespace Graphics;

MyPageSwitch::MyPageSwitch(QWidget *parent) :
    QWidget(parent)
{
    setFixedHeight(25);
    isFirstView = false;

    pagePosition = 0;
    selectedPage = NULL;
    initWidget();
}

void MyPageSwitch::initWidget()
{
    addPageButt = new QPushButton();
    addPageButt->setFixedSize(20,20);
    addPageButt->setToolTip("插入工作页(Ctrl+N)");
    addPageButt->setText("+");
    connect(addPageButt,SIGNAL(clicked()),this,SLOT(addPage()));

    layout = new QHBoxLayout;
    layout->setContentsMargins(1,1,1,1);
    layout->addWidget(addPageButt);
    layout->addStretch(0);
    setLayout(layout);
}

void MyPageSwitch::addSwitchContextMenu()
{
    MyMenu * rightMenu = MenuManager::instance()->createMenu(Constants::MENU_MYPAGE_SWITCH);
    rightMenu->addAction(ActionManager::instance()->action(Constants::CLOSE_WORKSPACE));
    rightMenu->addAction(ActionManager::instance()->action(Constants::SAVE_WORKSPACE));
    rightMenu->addAction(ActionManager::instance()->action(Constants::CLOSE_LEFT_WORKSPACE));
    rightMenu->addAction(ActionManager::instance()->action(Constants::CLOSE_RIGHT_WORKSPACE));
}

//添加页面
void MyPageSwitch::addPage()
{
    QString id = Util::getUUID();

    addPage(id);
}

//根据Id创建新场景
void MyPageSwitch::addPage(QString id,bool isAssociated)
{
    MyPageItem  * item = PageManager::instance()->addPageItem();
    connect(item,SIGNAL(switchPage(QString)),this,SLOT(switchToPage(QString)));
    connect(item,SIGNAL(deletePage(QString)),this,SLOT(deleteThisPage(QString)));
    item->setId(id);

    PageMapping * page = new PageMapping;
    page->id = id;
    page->pageItem = item;
    page->scene = MyGraphicsView::instance()->addScene(id);
    page->scene->setAssociation(isAssociated);
    page->scaleView = 100;
    page->isAssociated = isAssociated;
    page->hScrollValue = SceneWidth /2;
    page->vScrollValue = SceneHeight /2;
    page->pageName = QString("工作区%1").arg(PageManager::instance()->getPageCount());
    selectedPage = page;

    pages.append(page);
    layout->insertWidget(++pagePosition,item);
    item->setText(page->pageName);

    if(!isFirstView)
    {
        isFirstView = true;
        siwtchPage(id,isFirstView);
    }
    else
    {
        switchToPage(id);
    }
}

//根据id打开一个新的场景，如果不存在，则创建
bool MyPageSwitch::openPage(QString pageId)
{
    bool isExisted = false;
    foreach(PageMapping * mapping,pages)
    {
        if(mapping->id == pageId)
        {
            isExisted = true;
            break;
        }
    }

    if(isExisted)
    {
        siwtchPage(pageId);
    }
    else
    {
        addPage(pageId,true);
    }
    return isExisted;
}

//切换页面.切换前需要先保存当前scene中的尺寸信息
void MyPageSwitch::switchToPage(QString pageId)
{
    siwtchPage(pageId);
}

//切换页面
void MyPageSwitch::siwtchPage(QString pageId, bool firstView)
{
    if(selectedPage && !firstView)
    {
        selectedPage->hScrollValue = MyGraphicsView::instance()->getHorizonalValue();
        selectedPage->vScrollValue = MyGraphicsView::instance()->getVertiaclValue();
        selectedPage->scaleView = MyGraphicsView::instance()->getScaleValue();
    }

    foreach (PageMapping * mapping, pages)
    {
        if(mapping->id == pageId)
        {
            selectedPage = mapping;
            mapping->pageItem->setSelected(true);
            MyGraphicsView::instance()->transformView(mapping->hScrollValue,mapping->vScrollValue);
            MyGraphicsView::instance()->setScaleValue(mapping->scaleView);
            MyGraphicsView::instance()->showScene(mapping->scene);
            emit switchPage();
        }
        else
        {
            mapping->pageItem->setSelected(false);
        }
    }
}

//删除当前页面，用后面一个页面
void MyPageSwitch::deleteThisPage(QString pageId)
{
    int pageIndex = getPageIndex(pageId);

    QString tipInfo;

    if(pageIndex >= 0)
    {
        tipInfo = QString("是否关闭 [%1]?").arg(pages.at(pageIndex)->pageName);
    }
    else
    {
        tipInfo = "是否关闭此工作区?";
    }

    int result = Util::getWarnChoice(tipInfo);
    if(result == QMessageBox::Yes)
    {
        int index = -1;
        for( int i = 0; i < pages.size() ; i++)
        {
            if(pages.at(i)->id == pageId )
            {
                index = i;
                break;
            }
        }

        if( index >=0  && index < pages.size()-1)
        {
            switchToPage(pages.at(index + 1)->id);
        }
        else if(index == pages.size()-1 && index >0)
        {
            switchToPage(pages.at(index - 1)->id);
        }
        else if(index <0)
        {
            return;
        }

        pages.at(index)->scene->clear();

        delete pages.at(index)->scene;
        delete pages.at(index)->pageItem;
        pages.removeAt(index);

        --pagePosition;

        if(pages.size() == 0)
        {
            selectedPage = NULL;
            MyGraphicsView::instance()->deleteScene();
            emit switchPage();
        }
        emit deletePage();
    }
}

//获取当前页面的映射关系
const PageMapping *MyPageSwitch::currPageMapping()
{
    return selectedPage;
}

//切换前一个页面
void MyPageSwitch::switchFrontBack(bool isFront)
{
    MY_ASSERT_GT_ZERO(pages.size())

    int index = -1;
    for(int i = 0;i<pages.size(); i++)
    {
        if(selectedPage->id == pages.at(i)->id)
        {
            index = i;
            break;
        }
    }

    if(isFront && index >0)
    {
        switchToPage(pages.at(index -1 )->id);
    }
    else if(!isFront && index < pages.size() - 1)
    {
        switchToPage(pages.at(index +1 )->id);
    }
}

//关闭当前页
void MyPageSwitch::closePage()
{
    if(pages.size() > 0)
    {
        deleteThisPage(selectedPage->id);
    }
}

//关闭左侧所有工作区
void MyPageSwitch::closeLeftPage()
{
    int index = getPageIndex(selectedPage->id);
    if(index > 0)
    {
        for(int i = index - 1;i>=0;i--)
        {
            deleteThisPage(pages.at(i)->id);
        }
    }
}

//关闭右侧所有工作区
void MyPageSwitch::closeRightPage()
{
    int index = getPageIndex(selectedPage->id);

    if(index >= 0)
    {
        for(int i = pages.size() -1; i > index ;i--)
        {
            deleteThisPage(pages.at(i)->id);
        }
    }
}

//获取索引在集合中的位置
int MyPageSwitch::getPageIndex(QString id)
{
    for(int i = 0;i<pages.size();i++)
    {
        if(pages.at(i)->id == id)
        {
            return i;
        }
    }
    return -1;
}

//更新当前文件的描述信息
void MyPageSwitch::updateCurrMappingInfo(QString & fileName,MyItem * linkItem)
{
    MY_ASSERT(selectedPage)
    QFileInfo info(fileName);
    selectedPage->fullPathName = fileName;
    selectedPage->pathName = info.absolutePath();
#ifdef ADD_STATE_MODEL
    selectedPage->linkedItem = linkItem;
#endif
}

//手动的发射信号
void MyPageSwitch::emitSwitchPage()
{
    emit switchPage();
}

//获取当前页面的数量
int MyPageSwitch::count()const
{
    return pages.size();
}

//获取当前所有打开文件全路径列表
QStringList MyPageSwitch::getOpenedFullFileNames()
{
    QStringList list;

    foreach(PageMapping * mapping,pages)
    {
        list<<mapping->fullPathName;
    }

    return list;
}

//是否已经包含待打开的文件
bool MyPageSwitch::hasContainFile(QString & fileName)
{
    foreach(PageMapping * mapping,pages)
    {
        if(mapping->fullPathName == fileName)
        {
            return true;
        }
    }

    return false;
}

//将页面切换至此文件对应处
void MyPageSwitch::switchToPageByFileName(QString fileName)
{
    foreach(PageMapping * mapping,pages)
    {
        if(mapping->fullPathName == fileName)
        {
            siwtchPage(mapping->id);
            break;
        }
    }
}

MyPageSwitch * MyPageSwitch::pageSwitch = NULL;

MyPageSwitch * MyPageSwitch::instance()
{
    if(!pageSwitch)
    {
        pageSwitch = new MyPageSwitch;
    }
    return pageSwitch;
}

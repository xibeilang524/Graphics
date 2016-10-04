#include "mypageswitch.h"

#include <QHBoxLayout>
#include <QPushButton>
#include <QList>
#include <QMessageBox>
#include <QDebug>

#include "../manager/mypageitem.h"
#include "../util.h"
#include "Header.h"
#include "../item/myscene.h"
#include "../item/mygraphicsview.h"

MyPageSwitch::MyPageSwitch(QWidget *parent) :
    QWidget(parent)
{
    setFixedHeight(25);

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

//添加页面
void MyPageSwitch::addPage()
{
    QString id = Util::getUUID();

    MyPageItem  * item = PageManager::instance()->addPageItem();
    connect(item,SIGNAL(switchPage(QString)),this,SLOT(switchToPage(QString)));
    connect(item,SIGNAL(deletePage(QString)),this,SLOT(deleteThisPage(QString)));
    item->setId(id);

    PageMapping * page = new PageMapping;
    page->id = id;
    page->pageItem = item;
    page->scene = MyGraphicsView::instance()->addScene(id);
    selectedPage = page;

    pages.append(page);
    layout->insertWidget(1,item);
    item->setText(QString("工作区%1").arg(PageManager::instance()->getPageCount()));

    switchToPage(id);
}

//切换页面
void MyPageSwitch::switchToPage(QString pageId)
{
    foreach (PageMapping * mapping, pages)
    {
        if(mapping->id == pageId)
        {
            selectedPage = mapping;
            mapping->pageItem->setSelected(true);
            MyGraphicsView::instance()->showScene(mapping->scene);
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
    int result = QMessageBox::warning(0,"警告","是否删除此工作区?",QMessageBox::Yes,QMessageBox::No);
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
            pages.at(index+1)->pageItem->setSelected(true);
            MyGraphicsView::instance()->showScene(pages.at(index+1)->scene);
            selectedPage = pages.at(index + 1);
        }
        else if(index == pages.size()-1 && index >0)
        {
            pages.at(index - 1)->pageItem->setSelected(true);
            MyGraphicsView::instance()->showScene(pages.at(index - 1)->scene);
            selectedPage = pages.at(index - 1);
        }
        else if(index <0)
        {
            return;
        }

        pages.at(index)->scene->clear();

        delete pages.at(index)->scene;
        delete pages.at(index)->pageItem;
        pages.removeAt(index);

        if(pages.size() == 0)
        {
            MyGraphicsView::instance()->deleteScene();
        }
    }
}

//关闭当前页
void MyPageSwitch::closePage()
{
    deleteThisPage(selectedPage->id);
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

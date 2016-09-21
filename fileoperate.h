/*************************************************
**版  权：RenGu Company
**文件名: fileoperate.h
**作  者: wey       Version: 1.0       Date: 2016.09.09
**描  述: 控件保存解析
**Others:
**
**修改历史:
**20160909:wey:添加对控件的保存和解析
**20160920:wey:添加对端口的保存和解析
*************************************************/
#ifndef FILEOPERATE_H
#define FILEOPERATE_H

#include <QString>

class QGraphicsItem;

#include "Header.h"

class FileOperate
{
public:
    static FileOperate * instance();

    ReturnType saveFile(QString fileName, const QList<QGraphicsItem *> &items);
    ReturnType openFile(QString fileName, QList<CutInfo *> &items);

private:
    FileOperate();
    static FileOperate * fileOperate;
};

#endif // FILEOPERATE_H

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
**20160921:wey:添加对端口箭头的保存和解析
**20160929:wey:添加对解析文件中纵向深度的判定，查找出最大的深度，将其作为新拖入控件的深度值
**20161017:wey:修复控件保存-打开-保存多次，线条上的文字信息重复保存。
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

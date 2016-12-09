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
**20161020:wey:添加打开文件与当前版本的比对
**20161027:wey:添加D和三角端口读写
**20161028:wey:添加对端口文字内容读写
**20161115:wey:添加对服务的属性、循环的属性读写
**20161121:wey:添加对文件名版本号的写入，方便知道模型的版本
**20161209:wey:添加对折线的保存和读取，同时在剪切信息中添加折点集合
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

    ReturnType saveFile(QString &fileName, const QList<QGraphicsItem *> &items);
    ReturnType openFile(QString fileName, QList<CutInfo *> &items);

private:
    FileOperate();
    static FileOperate * fileOperate;
};

#endif // FILEOPERATE_H

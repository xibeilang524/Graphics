#include "serviceinputmodel.h"

#include <QModelIndex>
#include <QColor>
#include <QBrush>

#include <QDebug>

ServiceInputModel::ServiceInputModel(int row, int column):
    row(row),
    column(column)
{
    horizonalHeadList<<"参数名"<<"参数类型"<<"参数值"<<"备注信息";
}

//返回行数
int ServiceInputModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return paraList.size();
}

//返回列数
int ServiceInputModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return horizonalHeadList.size();
}

QVariant ServiceInputModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
    {
        return QVariant();
    }

    switch(role)
    {
         case Qt::DisplayRole:
                  {
                      if(paraList.size() > index.row())
                      {
                          switch(index.column())
                          {
                              case 0:
                                      return paraList.at(index.row())->pName;
                              case 1:
                                      return paraList.at(index.row())->pType;
                              case 2:
                                      return paraList.at(index.row())->pValue;
                              case 3:
                                      return paraList.at(index.row())->pRemark;
                          }
                      }
                      break;
                  }
         case Qt::FontRole:
                  {
                      QColor color;
                      color.setRgb(255,0,0);
                      return color;
                  }
        default:
             break;
    }
    return QVariant();
}

//设置表头的样式
QVariant ServiceInputModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if( role == Qt::BackgroundColorRole)
    {
        if( orientation == Qt::Horizontal)
        {
            QBrush brush(QColor(255,0,0));
            return brush;
        }
    }
    else if(role == Qt::DisplayRole)
    {
        if( orientation == Qt::Horizontal)
        {
            if(horizonalHeadList.size() == column)
            {
                return horizonalHeadList.at(section);
            }
        }
        else if(orientation == Qt::Vertical)
        {
            return QString("%1").arg(section +1);
        }
    }
    return QVariant();
}

QModelIndex ServiceInputModel::index(int row, int column, const QModelIndex &parent) const
{
    if(parent.isValid())
    {
        return parent.child(row,column);
    }
    return this->createIndex(row,column);
}

//设置属性
Qt::ItemFlags ServiceInputModel::flags(const QModelIndex &index) const
{
    Q_UNUSED(index)
    return Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

//设置数据
bool ServiceInputModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(index.isValid() && role == Qt::EditRole)
    {
        switch(index.column())
        {
            case 0:
                   paraList.at(index.row())->pName = value.toString();
                   break;
            case 1:
                   paraList.at(index.row())->pType = value.toString();
                   break;
            case 2:
                   paraList.at(index.row())->pValue = value.toString();
                   break;
            case 3:
                   paraList.at(index.row())->pRemark = value.toString();
                   break;
            default:
                 break;
        }
        return true;
    }
    return false;
}

//添加一行
bool ServiceInputModel::addRow()
{
    beginInsertRows(QModelIndex(),paraList.size()+1,paraList.size()+1);

    Parameter * para = new Parameter;
    para->pName = "";
    para->pType = "";
    para->pValue = "";
    para->pRemark = "";
    paraList.append(para);

    endInsertRows();
    return true;
}

//增加一行带参数
bool ServiceInputModel::addRow(Parameter * intputPara)
{
    beginInsertRows(QModelIndex(),paraList.size()+1,paraList.size()+1);

    Parameter * para = new Parameter;
    para->pName = intputPara->pName;
    para->pType = intputPara->pType;
    para->pValue = "";
    para->pRemark = intputPara->pRemark;
    paraList.append(para);

    endInsertRows();
    return true;
}

//删除行
bool ServiceInputModel::deleteRow(int rowNum)
{
    if(rowNum < paraList.size())
    {
        beginRemoveRows(QModelIndex(),rowNum,rowNum);
        delete paraList.at(rowNum);
        paraList.removeAt(rowNum);
        endRemoveRows();
        return true;
    }
    return false;
}

//初始设置,每次窗口显示的时均为新建，因此需要手动的添加行显示数据
void ServiceInputModel::setPara(ParaList list)
{
    paraList = list;

    if(paraList.size() > 0)
    {
        beginInsertRows(QModelIndex(),0,paraList.size()-1);

        endInsertRows();
    }
}

//清空表格内容
void ServiceInputModel::clearTable()
{
    foreach(Parameter * para,paraList)
    {
        delete para;
    }

    if(paraList.size() > 0)
    {
        beginRemoveRows(QModelIndex(),0,paraList.size() - 1);

        endRemoveRows();
    }

    paraList.clear();
}


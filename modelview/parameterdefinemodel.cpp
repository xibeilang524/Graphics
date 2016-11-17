#include "parameterdefinemodel.h"

#include <QDebug>

ParameterDefineModel::ParameterDefineModel(const LoopPart loopType, int row, int column):
    loopPart(loopType),
    row(row),
    column(column)
{
    switch(loopPart)
    {
        case LOOP_VARI:
                        horizonalHeadList<<"类型名"<<"变量名"<<"值"<<"备注信息";
                        break;
        case LOOP_EXPRESS:
                        horizonalHeadList<<"变量名"<<"关系运算符"<<"值"<<"备注信息";
                        break;
        case LOOP_FINAL:
                        horizonalHeadList<<"变量名"<<"操作运算符"<<"值"<<"备注信息";
                        break;
    }
}

//返回行数
int ParameterDefineModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    if(loopPart == LOOP_VARI)
    {
        return paraList.size();
    }
    else if(loopPart == LOOP_EXPRESS)
    {
        return expList.size();
    }
    else if(loopPart == LOOP_FINAL)
    {
        return fexpList.size();
    }
    return 0;
}

//返回列数
int ParameterDefineModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return horizonalHeadList.size();
}

QVariant ParameterDefineModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
    {
        return QVariant();
    }

    switch(role)
    {
         case Qt::DisplayRole:
                  {
                        if(loopPart == LOOP_VARI)
                        {
                            if(paraList.size() > index.row())
                            {
                                switch(index.column())
                                {
                                    case 0:
                                            return paraList.at(index.row())->type;
                                    case 1:
                                            return paraList.at(index.row())->name;
                                    case 2:
                                            return paraList.at(index.row())->value;
                                    case 3:
                                            return paraList.at(index.row())->pRemark;
                                }
                            }
                        }
                        else if(loopPart == LOOP_EXPRESS)
                        {
                            if(expList.size() > index.row())
                            {
                                switch(index.column())
                                {
                                    case 0:
                                            return expList.at(index.row())->name;
                                    case 1:
                                            return expList.at(index.row())->expressType;
                                    case 2:
                                            return expList.at(index.row())->value;
                                    case 3:
                                            return expList.at(index.row())->pRemark;
                                }
                            }
                        }
                        else if(loopPart == LOOP_FINAL)
                        {
                            if(fexpList.size() > index.row())
                            {
                                switch(index.column())
                                {
                                    case 0:
                                            return fexpList.at(index.row())->name;
                                    case 1:
                                            return fexpList.at(index.row())->expressType;
                                    case 2:
                                            return fexpList.at(index.row())->value;
                                    case 3:
                                            return fexpList.at(index.row())->pRemark;
                                }
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
QVariant ParameterDefineModel::headerData(int section, Qt::Orientation orientation, int role) const
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

QModelIndex ParameterDefineModel::index(int row, int column, const QModelIndex &parent) const
{
    if(parent.isValid())
    {
        return parent.child(row,column);
    }
    return this->createIndex(row,column);
}

//设置属性
Qt::ItemFlags ParameterDefineModel::flags(const QModelIndex &index) const
{
    Q_UNUSED(index)
    return Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

//设置数据
bool ParameterDefineModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(index.isValid() && role == Qt::EditRole)
    {
        if( loopPart == LOOP_VARI )
        {
            switch(index.column())
            {
                case 0:
                       paraList.at(index.row())->type = value.toString();
                       break;
                case 1:
                       paraList.at(index.row())->name = value.toString();
                       emit variNumEdited();
                       break;
                case 2:
                       paraList.at(index.row())->value = value.toString();
                       break;
                case 3:
                       paraList.at(index.row())->pRemark = value.toString();
                       break;
                default:
                     break;
            }
        }
        else if( loopPart == LOOP_EXPRESS )
        {
            switch(index.column())
            {
                case 0:
                       expList.at(index.row())->name = value.toString();
                       break;
                case 1:
                       expList.at(index.row())->expressType = value.toString();
                       break;
                case 2:
                       expList.at(index.row())->value = value.toString();
                       break;
                case 3:
                       expList.at(index.row())->pRemark = value.toString();
                       break;
                default:
                     break;
            }
        }
        else if( loopPart == LOOP_FINAL )
        {
            switch(index.column())
            {
                case 0:
                       fexpList.at(index.row())->name = value.toString();
                       break;
                case 1:
                       fexpList.at(index.row())->expressType = value.toString();
                       break;
                case 2:
                       fexpList.at(index.row())->value = value.toString();
                       break;
                case 3:
                       fexpList.at(index.row())->pRemark = value.toString();
                       break;
                default:
                     break;
            }
        }
        return true;
    }
    return false;
}

//添加一行
bool ParameterDefineModel::addRow()
{
    if(loopPart == LOOP_VARI)
    {
        beginInsertRows(QModelIndex(),paraList.size()+1,paraList.size()+1);

        VariableDefine * para = new VariableDefine;
        para->type = "";
        para->name = "";
        para->value = "";
        paraList.append(para);

        endInsertRows();
    }
    else if(loopPart == LOOP_EXPRESS)
    {
        beginInsertRows(QModelIndex(),expList.size()+1,expList.size()+1);

        ExpressDefine * para = new ExpressDefine;
        para->name = "";
        para->expressType = "";
        para->value = "";
        expList.append(para);

        endInsertRows();
    }
    else if(loopPart == LOOP_FINAL)
    {
        beginInsertRows(QModelIndex(),fexpList.size()+1,fexpList.size()+1);

        FinalExpressDefine * para = new FinalExpressDefine;
        para->name = "";
        para->expressType = "";
        para->value = "";
        fexpList.append(para);

        endInsertRows();
    }

    return true;
}

//增加一行变量
bool ParameterDefineModel::addRow(VariableDefine * intputPara)
{
    beginInsertRows(QModelIndex(),paraList.size()+1,paraList.size()+1);

    VariableDefine * para = new VariableDefine;
    para->type = intputPara->type;
    para->name = intputPara->name;
    para->value = "";
    paraList.append(para);

    endInsertRows();
    return true;
}

//增加一行条件
bool ParameterDefineModel::addRow(ExpressDefine * intputPara)
{
    beginInsertRows(QModelIndex(),paraList.size()+1,paraList.size()+1);

    ExpressDefine * para = new ExpressDefine;
    para->name = intputPara->name;
    para->expressType = intputPara->expressType;
    para->value = "";
    expList.append(para);

    endInsertRows();
    return true;
}

//增加一行末尾条件
bool ParameterDefineModel::addRow(FinalExpressDefine * intputPara)
{
    beginInsertRows(QModelIndex(),paraList.size()+1,paraList.size()+1);

    FinalExpressDefine * para = new FinalExpressDefine;
    para->name = intputPara->name;
    para->expressType = intputPara->expressType;
    para->value = "";
    fexpList.append(para);

    endInsertRows();
    return true;
}

//删除行
bool ParameterDefineModel::deleteRow(int rowNum)
{
    int totalRow = 0;
    if(loopPart == LOOP_VARI)
    {
        totalRow = paraList.size();
    }
    else if(loopPart == LOOP_EXPRESS)
    {
        totalRow = expList.size();
    }
    else if(loopPart == LOOP_FINAL)
    {
        totalRow = fexpList.size();
    }

    if(rowNum < totalRow)
    {
        beginRemoveRows(QModelIndex(),rowNum,rowNum);
        if(loopPart == LOOP_VARI )
        {
            emit deleteVariInfo(paraList.at(rowNum)->name);
            delete paraList.at(rowNum);
            paraList.removeAt(rowNum);
        }
        else if(loopPart == LOOP_EXPRESS)
        {
            delete expList.at(rowNum);
            expList.removeAt(rowNum);
        }
        else if(loopPart == LOOP_FINAL)
        {
            delete fexpList.at(rowNum);
            fexpList.removeAt(rowNum);
        }

        endRemoveRows();
        return true;
    }
    return false;
}

//根据名称删除行
void ParameterDefineModel::deleteRow(QString name)
{
    if(loopPart == LOOP_VARI)
    {

    }
    else if(loopPart == LOOP_EXPRESS)
    {
        for(int i = 0;i<expList.size();i++)
        {
            if(expList.at(i)->name == name)
            {
                deleteRow(i);
                break;
            }
        }
    }
    else if(loopPart == LOOP_FINAL)
    {
        for(int i = 0;i<fexpList.size();i++)
        {
            if(fexpList.at(i)->name == name)
            {
                deleteRow(i);
                break;
            }
        }
    }
}

//初始设置,每次窗口显示的时均为新建，因此需要手动的添加行显示数据
void ParameterDefineModel::setParameterProp(LoopProperty * prop)
{
    int totalSize = 0;
    if(loopPart == LOOP_VARI)
    {
        foreach(VariableDefine * tmp,prop->varList)
        {
            VariableDefine * newVari = new VariableDefine;
            newVari->name = tmp->name;
            newVari->type = tmp->type;
            newVari->value = tmp->value;
            newVari->pRemark = tmp->pRemark;
            paraList.push_back(newVari);
        }
        totalSize =  prop->varList.size();
    }
    else if(loopPart == LOOP_EXPRESS)
    {
        foreach(ExpressDefine * tmp,prop->expList)
        {
            ExpressDefine * newExp = new ExpressDefine;
            newExp->name = tmp->name;
            newExp->expressType = tmp->expressType;
            newExp->value = tmp->value;
            newExp->pRemark = tmp->pRemark;
            expList.push_back(newExp);
        }
        totalSize =  prop->expList.size();
    }
    else if(loopPart == LOOP_FINAL)
    {
        foreach(FinalExpressDefine * tmp,prop->fexpList)
        {
            FinalExpressDefine * newFexp = new FinalExpressDefine;
            newFexp->name = tmp->name;
            newFexp->expressType = tmp->expressType;
            newFexp->value = tmp->value;
            newFexp->pRemark = tmp->pRemark;
            fexpList.push_back(newFexp);
        }
        totalSize =  prop->fexpList.size();
    }

    if(totalSize > 0)
    {
        beginInsertRows(QModelIndex(),0,totalSize-1);

        endInsertRows();
    }
}

//清空表格内容
void ParameterDefineModel::clearTable()
{
    if(loopPart == LOOP_VARI)
    {
        foreach(VariableDefine * para,paraList)
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
    else if(loopPart == LOOP_VARI)
    {
        foreach(ExpressDefine * para,expList)
        {
            delete para;
        }

        if(expList.size() > 0)
        {
            beginRemoveRows(QModelIndex(),0,expList.size() - 1);

            endRemoveRows();
        }

        expList.clear();
    }
    else if(loopPart == LOOP_VARI)
    {
        foreach(FinalExpressDefine * para,fexpList)
        {
            delete para;
        }

        if(fexpList.size() > 0)
        {
            beginRemoveRows(QModelIndex(),0,fexpList.size() - 1);

            endRemoveRows();
        }

        fexpList.clear();
    }
}

ParameterDefineModel::~ParameterDefineModel()
{
    foreach(VariableDefine * tmp,paraList)
    {
        delete tmp;
    }

    foreach(ExpressDefine * tmp,expList)
    {
        delete tmp;
    }

    foreach(FinalExpressDefine * tmp,fexpList)
    {
        delete tmp;
    }
}

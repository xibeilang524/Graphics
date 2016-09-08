#ifndef RIGHTTOOLBOX_H
#define RIGHTTOOLBOX_H

#include <QWidget>
#include <QPen>
#include <QBrush>

#include "Header.h"

class QLabel;

namespace Ui {
class RightToolBox;
}

class RightToolBox : public QWidget
{
    Q_OBJECT
    
public:
    explicit RightToolBox(QWidget *parent = 0);
    ~RightToolBox();

signals:
    void updateProperty(ItemProperty property);           //将新的属性设置到选中的item之上
    void deleteCurrItem();

private slots:
    void respInitToolBox(int seletedItemNum,ItemProperty property);
    void respItemPosChanged(MyRect rect);

    void changeFillColor();              //改变填充颜色
    void opacityChange(int value);       //改变透明度
    void isViewFill(bool flag);          //是否填充
    void chooseFillColor();              //显示颜色选择对话框
    void fillKindChange(int index);      //笔刷填充改变
    void isViewBorder(bool flag);        //是否显示边框
    void chooseLineColor();              //显示线条颜色选择对话框
    void borderKindChange(int i);        //边框类型改变
    void borderWidthChange(int i);       //边框宽度改变
    void sizeChange();                   //改变尺寸
    void positionChange();               //改变位置
    void degreeChange();                 //改变角度
    void changeDegree(int value);        //旋转diral
    void deleteItem();                   //删除item
    void chooseFont();                   //选择字体
    void chooseFontColor();              //显示字体颜色选择对话框
    
private:
    void initConnection();
    void setFillColor();
    void setLineColor();
    void enableButtState(bool isEnabled);
    void prepareBrush();                 //准备笔刷
    void preparePen();                   //准备画笔
    void setNum(QLabel *edit, bool minus,int &newValue);
    int getSizeValue(QLabel* edit,int pos);
    void updateItemProperty();

    Ui::RightToolBox *ui;

    bool isDataInit;                     //是否是数据的初始化时设置滑动条值，那么就不要再发信号了。

    QBrush newBrush;
    QColor selectedFillColor;
    QColor selectedLineColor;
    QColor selectedFontColor;
    QColor initalFillColor,initalLineColor,initalFontColor;  //用于在弹出的颜色选择对话框中设置初始的颜色
    QPen newPen;

    ItemProperty currItemProperty;
};

#endif // RIGHTTOOLBOX_H

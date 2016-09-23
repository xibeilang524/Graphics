#include "myslider.h"

#include <QPushButton>
#include <QSlider>
#include <QHBoxLayout>
#include <QLabel>
#include <QEvent>
#include <QMouseEvent>

#include <QDebug>

MySlider::MySlider(QWidget * parent):
    QWidget(parent)
{
    minValue = 10;
    maxValue = 500;
    currValue = 100;
    preValue = 0;
    step = 10;

    initWidget();
    this->setFixedSize(360,25);
}

void MySlider::initWidget()
{
    subButt = new QPushButton(this);
    subButt->setText("-");
    subButt->setFixedSize(25,25);
    subButt->setObjectName("subButt");
    subButt->setToolTip("缩小");
    connect(subButt,SIGNAL(clicked()),this,SLOT(respZoomOut()));

    addButt = new QPushButton(this);
    addButt->setText("+");
    addButt->setFixedSize(25,25);
    addButt->setObjectName("addButt");
    addButt->setToolTip("放大");
    connect(addButt,SIGNAL(clicked()),this,SLOT(respZoomIn()));

    slider = new QSlider(Qt::Horizontal,this);
    slider->setMinimum(minValue);
    slider->setMaximum(maxValue);
    slider->setValue(100);
    slider->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    connect(slider,SIGNAL(valueChanged(int)),this,SLOT(respSliderChanged(int)));

    label = new QLabel(this);
    label->setFixedSize(80,25);
    label->setAlignment(Qt::AlignCenter);
    label->setToolTip("恢复默认比例");
    setLabelText(currValue);
    label->installEventFilter(this);

    QHBoxLayout * layout = new QHBoxLayout;
    layout->addWidget(subButt);
    layout->addWidget(slider);
    layout->addWidget(addButt);
    layout->addWidget(label);
    layout->setContentsMargins(1,1,1,1);
    this->setLayout(layout);
}

//放大
void MySlider::respZoomIn()
{
    if(currValue < minValue || currValue > maxValue)
    {
        return;
    }
    if(currValue < minValue || currValue > maxValue)
    {
        return;
    }
    if(currValue % step !=0)
    {
        currValue = (currValue/step+1)*step;
    }
    else
    {
        currValue += step;
    }

    respButtClick();
}

//缩小
void MySlider::respZoomOut()
{
    if(currValue % step !=0)
    {
        currValue = (currValue/step)*step;
    }
    else
    {
        currValue -= step;
    }
    respButtClick();
}

//响应鼠标点击事件
void MySlider::respButtClick()
{
    if(currValue < minValue)
    {
        currValue = minValue;
    }
    else if(currValue > maxValue)
    {
        currValue = maxValue;
    }

    if(currValue == preValue)
    {
        return;
    }

    preValue = currValue;
    slider->setValue(currValue);
}

bool MySlider::eventFilter(QObject *obj, QEvent *event)
{
    if(obj == label)
    {
        if(event->type() == QEvent::MouseButtonPress)
        {
            QMouseEvent * eve = dynamic_cast<QMouseEvent *>(event);
            if(eve->button() == Qt::LeftButton)
            {
                currValue = 100;
                slider->setValue(currValue);
            }
        }
        else if(event->type() == QEvent::Enter)
        {
            this->setCursor(Qt::PointingHandCursor);
        }
        else if(event->type() == QEvent::Leave)
        {
            this->setCursor(Qt::ArrowCursor);
        }
    }
    return QWidget::eventFilter(obj,event);
}

void MySlider::setSliderRange(const int min, const int max)
{
    if(min<0||max>500||min>=max)
    {
        return;
    }

    this->minValue = min;
    this->maxValue = max;
}

void MySlider::setStep(int step)
{
    if(step <= 0 || step > maxValue)
    {
        step = 10;
        return;
    }
    this->step = step;
}

void MySlider::respSliderChanged(int value)
{
    currValue = value;
    setLabelText(currValue);
    emit scaleView(value);
}

void MySlider::setLabelText(int value)
{
    label->setText(QString::number(value)+"%");
}

MySlider::~MySlider()
{
    delete subButt;
    delete addButt;
    delete slider;
    delete label;
}

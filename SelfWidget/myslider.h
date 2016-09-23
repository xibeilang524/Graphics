#ifndef MYSLIDER_H
#define MYSLIDER_H

#include <QWidget>

class QPushButton;
class QSlider;
class QLabel;

class MySlider : public QWidget
{
    Q_OBJECT
public:
    MySlider(QWidget * parent = 0);
    void setSliderRange(const int min,const int max);
    void setStep(int step);
    ~MySlider();

signals:
    void scaleView(int);

protected:
    bool eventFilter(QObject *obj, QEvent *event);

private slots:
    void respButtClick();
    void respSliderChanged(int);
    void respZoomIn();
    void respZoomOut();

private:
    void setLabelText(int value);
    void initWidget();

    int minValue,maxValue;
    int currValue,preValue;
    int step;

    QPushButton * subButt;
    QPushButton * addButt;

    QSlider * slider;
    QLabel * label;
};

#endif // MYSLIDER_H

/*************************************************
**版  权：RenGu Company
**文件名: myprowizard.h
**作  者: wey       Version: 1.0       Date: 2016.11.09
**描  述: 新建工程页面
**Others:
**
**修改历史:
**
*************************************************/
#ifndef MYPROWIZARD_H
#define MYPROWIZARD_H

#include <QWizard>

class MyWelcomePage;
class MyFinalDescPage;

class MyProWizard : public QWizard
{
    Q_OBJECT
public:
    static MyProWizard * instance();
//    QString  getFillInfo();

protected:
    void accept();

signals:
    void proInfo(QString proPath,QString proName);

private:
    MyProWizard(QWidget * parent = 0);
    static MyProWizard * wizard;

    void createSubPage();

    enum PageIndex
    {
        WELCOME_PAGE,
        FINALDESC_PAGE
    };

    MyWelcomePage * welcomePage;
    MyFinalDescPage * finalDescPage;

    QString fillInfo;

};

#endif // MYPROWIZARD_H

/*************************************************
**版  权：RenGu Company
**文件名: mywelcomepage.h
**作  者: wey       Version: 1.0       Date: 2016.11.09
**描  述: 欢迎页面
**Others:
**
**修改历史:
**
*************************************************/
#ifndef MYWELCOMEPAGE_H
#define MYWELCOMEPAGE_H

#include <QWizardPage>

namespace Ui {
class MyWelcomePage;
}

class MyWelcomePage : public QWizardPage
{
    Q_OBJECT
    
public:
    explicit MyWelcomePage(QWidget *parent = 0);
    ~MyWelcomePage();

protected:
    bool isComplete() const;
    
private slots:
    void showFilePathSelect();
    void validatePro(QString);

private:
    void showError(QString text);

    Ui::MyWelcomePage *ui;

    bool isInfoFilled;              //信息是否填写完整
};

#endif // MYWELCOMEPAGE_H

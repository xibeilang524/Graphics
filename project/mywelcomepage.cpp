#include "mywelcomepage.h"
#include "ui_mywelcomepage.h"

#include <QFileDialog>
#include <QFileInfo>
#include <QDir>

MyWelcomePage::MyWelcomePage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::MyWelcomePage)
{
    ui->setupUi(this);

    setTitle("项目介绍和位置");
    setSubTitle("This wizard generates a complex model project.");
    setWindowIcon(QIcon(":/images/softicon.png"));

    ui->error->hide();
    ui->proPath->setEnabled(false);
    isInfoFilled = false;

    connect(ui->viewPath,SIGNAL(clicked()),this,SLOT(showFilePathSelect()));
    connect(ui->proName,SIGNAL(textChanged(QString)),this,SLOT(validatePro(QString)));
    connect(ui->proPath,SIGNAL(textChanged(QString)),this,SLOT(validatePro(QString)));

    registerField("proName",ui->proName);
    registerField("proPath",ui->proPath);
}

//用于控制下一页是否可用
bool MyWelcomePage::isComplete()const
{
    return isInfoFilled;
}

//验证工程名和路径是否正确
void MyWelcomePage::validatePro(QString)
{
    QString proName = ui->proName->text();
    QString proPath = ui->proPath->text();

    if(proName.isEmpty() || proName.contains("\\s+"))
    {
        showError("工程名不合法");
        return;
    }

    if(proPath.isEmpty() || proName.contains("\\s+"))
    {
        showError("路径名不合法");
        return;
    }

    QString fullName = proPath + "/" + proName;
    QFileInfo info(fullName);
    if(info.exists())
    {
        showError("工程已经存在");
        return;
    }

    ui->error->hide();

    isInfoFilled = true;
    emit completeChanged();
}

//显示工程路径选择框
void MyWelcomePage::showFilePathSelect()
{
    QString selectedPath = QFileDialog::getExistingDirectory(this,"选择目录","/home",QFileDialog::ShowDirsOnly| QFileDialog::DontResolveSymlinks);
    if(!selectedPath.isEmpty())
    {
        ui->proPath->setText(selectedPath);
    }
}

void MyWelcomePage::showError(QString text)
{
    ui->error->show();
    ui->error->setStyleSheet("background-color:red;color:white;");
    ui->error->setText(text);
    isInfoFilled = false;
    emit completeChanged();
}

MyWelcomePage::~MyWelcomePage()
{
    delete ui;
}

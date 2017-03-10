#include "admin_center.h"
#include "ui_admin_center.h"

#include <QProcess>
#include <QGraphicsOpacityEffect>

Admin_Center::Admin_Center(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Admin_Center)
{
    ui->setupUi(this);

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(time_slot()));
    timer->start(1000);

    QPalette lcdpat = ui->lcdNumber->palette();
    /*设置颜色，整体背景颜色 颜色蓝色,此函数的第一个参数可以设置多种。如文本、按钮按钮文字、多种*/
    lcdpat.setColor(QPalette::Normal,QPalette::WindowText,Qt::blue);
    //设置当前窗口的调色板
    ui->lcdNumber->setPalette(lcdpat);
    //设置背景色
    //ui->lcdNumber->setStyleSheet("background-color: white");
    ui->lcdNumber->setStyleSheet("background-image: url(:imaBackground/background.jpg)");
    ui->lcdNumber->display(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));

    time_slot();


    connect(ui->btnUserInfo, SIGNAL(clicked()), this, SLOT(to_userInfo()));
    connect(ui->btnComment, SIGNAL(clicked()), this, SLOT(to_commentInfo()));
    connect(ui->btnModifyPwd, SIGNAL(clicked()), this, SLOT(to_modify_pwd()));
    connect(ui->btnTime, SIGNAL(clicked()), this, SLOT(to_setting_time()));
    connect(ui->btnExit,SIGNAL(clicked()), this, SLOT(close()));

    setWindowModality(Qt::ApplicationModal); //设置模态

    QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect();
    effect->setOpacity(0.7);
    ui->btnUserInfo->setGraphicsEffect(effect);
    ui->btnComment->setGraphicsEffect(effect);
    ui->btnExit->setGraphicsEffect(effect);
    ui->btnModifyPwd->setGraphicsEffect(effect);
    ui->btnTime->setGraphicsEffect(effect);


    this->setAutoFillBackground(true);
    QPalette palette;
    QPixmap pixmap(":/imgBackground/background.jpg");
    pixmap.scaled(this->size());
    palette.setBrush(QPalette::Window, QBrush(pixmap));
    this->setPalette(palette);


    this->setFixedSize(this->width(), this->height());
    this->showFullScreen();
    this->clearFocus();
}

Admin_Center::~Admin_Center()
{
    delete ui;
}


/**************slot**************/

//显示系统时间
void Admin_Center::time_slot()
{
    ui->lcdNumber->setDigitCount(19);
    ui->lcdNumber->setSegmentStyle(QLCDNumber::Flat);
    ui->lcdNumber->display((new QDateTime) ->currentDateTime()
                           .toString("yyyy-MM-dd hh:mm:ss"));
}

void Admin_Center::to_userInfo()
{
    manage_user_info = new Manage_User_Info();
    manage_user_info->initInterface();
    manage_user_info->showFullScreen();
    manage_user_info->show();
}

void Admin_Center::to_commentInfo()
{
    manage_comment_info = new Manage_Comment_Info();
    manage_comment_info->show();
}

void Admin_Center::to_modify_pwd()
{
    admin_modify_pwd = new Admin_Modify_Pwd();
    connect(admin_modify_pwd, SIGNAL(success(Member_Info)), this, SLOT(to_reshow(Member_Info)));
    admin_modify_pwd->setAdminInfo(admin_info);
    admin_modify_pwd->show();
}

void Admin_Center::to_reshow(Member_Info admin_info)
{
    this->admin_info = admin_info;
}

void Admin_Center::to_setting_time()
{
    time_setting = new Time_Setting();
    time_setting->show();
}

/********************************/


void Admin_Center::setAdminInfo(QString adminId, QString pwd)
{
    admin_info.setId(adminId);
    admin_info.setPwd(pwd);
}

void Admin_Center::closeEvent(QCloseEvent *event)
{
    emit to_close();
    QWidget::closeEvent(event);
}

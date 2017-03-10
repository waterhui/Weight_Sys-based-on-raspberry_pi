#include "widget.h"
#include "ui_widget.h"
#include <QDebug>

#include <QGraphicsDropShadowEffect>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    hx711::Setup();
    key = new keyBoard();
    key->setWindowModality(Qt::ApplicationModal);
    key->setGeometry(0,300,1024,300);


    leUsr = new QLineEdit(this);
    leUsr->setObjectName("leUsr");
    leUsr->setMaxLength(20);
    leUsr->setGeometry(410,183,201,33);
    leUsr->setStyleSheet("font-size : 15px");
    leUsr->installEventFilter(this);

    lePwd = new QLineEdit(this);
    lePwd->setObjectName("lePwd");
    lePwd->setMaxLength(20);
    lePwd->setEchoMode(QLineEdit::Password);
    lePwd->setGeometry(410,240,201,33);
    lePwd->setStyleSheet("font-size : 15px");
    lePwd->installEventFilter(this);

    data_helper = new dataHelper();

    connect(key,SIGNAL(Hide()),this,SLOT(reFocus()));
    connect(key,SIGNAL(Edit(QString)),this,SLOT(Edit(QString)));

    key->hide();


    connect(ui->btnRegister,SIGNAL(clicked()),this,SLOT(toReg()));
    connect(ui->btnTourTest,SIGNAL(clicked()),this,SLOT(to_measure()));
    connect(ui->btnLogin, SIGNAL(clicked()), this, SLOT(toLogin()));

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(time_slot()));
    timer->start(1000);

    QPalette lcdpat = ui->lcdNumber->palette();
    /*设置颜色，整体背景颜色 颜色蓝色,此函数的第一个参数可以设置多种。如文本、按钮按钮文字、多种*/
    lcdpat.setColor(QPalette::Normal,QPalette::WindowText,Qt::blue);
    //设置当前窗口的调色板
    ui->lcdNumber->setPalette(lcdpat);
    //设置背景色
    ui->lcdNumber->setStyleSheet("background-color: white");
    ui->lcdNumber->setStyleSheet("background-image: url(:imaBackground/background.jpg)");
    ui->lcdNumber->display(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));

    time_slot();

    this->setAutoFillBackground(true);
    QPalette palette;
    QPixmap pixmap(":/imgBackground/background.jpg");
    pixmap.scaled(this->size());
    palette.setBrush(QPalette::Window, QBrush(pixmap));
    this->setPalette(palette);

    this->setFixedSize(this->width(), this->height());


    this->leUsr->setPlaceholderText("请输入用户名");
    this->lePwd->setPlaceholderText("请输入密码");


   QGraphicsDropShadowEffect *shadow_effect_1 = new QGraphicsDropShadowEffect(this);
   QGraphicsDropShadowEffect *shadow_effect_2 = new QGraphicsDropShadowEffect(this);

   shadow_effect_1->setOffset(5, 5);
   shadow_effect_1->setColor(Qt::gray);
   shadow_effect_1->setBlurRadius(8);
   shadow_effect_2->setOffset(5, 5);
   shadow_effect_2->setColor(Qt::gray);
   shadow_effect_2->setBlurRadius(8);

   this->leUsr->setGraphicsEffect(shadow_effect_1);
   this->lePwd->setGraphicsEffect(shadow_effect_2);
   this->showFullScreen(); //设置为全屏

    ui->btnLogin->setFocus();
}

Widget::~Widget()
{
    delete data_helper;
    delete ui;
}

/****************************slots*******************************/
void Widget::to_measure()
{
    tourT = new tourTest();
    tourT->setAttribute(Qt::WA_DeleteOnClose, true);
    tourT->show();
    tourT->showFullScreen();
}

void Widget::reFocus()
{
    ui->btnLogin->setFocus();
    key->hide();
}

//编辑两个文本框
void Widget::Edit(QString s)
{
    this->le_select->setText(s);
}

//注册
void Widget::toReg()
{
    reg =  new Register();
    ui->btnLogin->setFocus();
    reg->setAttribute(Qt::WA_DeleteOnClose, true);
    reg->showFullScreen();
    reg->show();

}

//登录
void Widget::toLogin()
{
    Message_Box *message_box;
    //判断用户名和密码是否为空
    if (leUsr->text().trimmed() == "" || lePwd->text().trimmed() == "" ) {
        message_box = new Message_Box();
        message_box->setTitle("注意");
        message_box->setText("用户名和密码不能为空!");
        message_box->setShowButton(false, true);
        message_box->setImg(Message_Box::Warning);
        message_box->exec();
        return;
    }

    //执行查询
    QString select = QString("select * from Login where Id = '%1' and Pwd = '%2'")
            .arg(leUsr->text().trimmed())
            .arg(lePwd->text().trimmed());
    bool respone = data_helper->Select(select);
    if(respone) {
        //如果是系统管理员
        if (leUsr->text().trimmed() == "admin") {
            admin_ctr = new Admin_Center();
            admin_ctr->setAdminInfo(this->leUsr->text().trimmed(), this->lePwd->text().trimmed());
            this->leUsr->clear();
            this->lePwd->clear();
            connect(admin_ctr, SIGNAL(to_close()), this, SLOT(show()));
            admin_ctr->setAttribute(Qt::WA_DeleteOnClose, true);
            admin_ctr->show();
            this->hide();
        } else {
            mem_ctr = new Member_Center();
            connect(mem_ctr, SIGNAL(to_close()), this, SLOT(show()));
            mem_ctr->setUserInfo(leUsr->text().trimmed(), lePwd->text().trimmed());
            mem_ctr->initInterface();
            this->leUsr->clear();
            this->lePwd->clear();
            mem_ctr->setAttribute(Qt::WA_DeleteOnClose, true);
            mem_ctr->showFullScreen();
            mem_ctr->show();
            this->hide();
        }
    } else {
        message_box = new Message_Box();
        message_box->setTitle("注意");
        message_box->setShowButton(false, true);
        message_box->setText("用户名或者密码错误");
        message_box->setImg(Message_Box::Warning);
        message_box->exec();
    }
}

void Widget::time_slot()
{
    ui->lcdNumber->setDigitCount(19);
    ui->lcdNumber->setSegmentStyle(QLCDNumber::Flat);
    ui->lcdNumber->display((new QDateTime) ->currentDateTime()
                           .toString("yyyy-MM-dd hh:mm:ss"));
}

/**************************************************************/

bool Widget::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == this->leUsr || obj == this->lePwd) {
        if (event->type() == QEvent::FocusIn) {
            le_select = (QLineEdit *) obj;
            key->setLeText(le_select->text());
            if (le_select == lePwd)
                key->SetPwdMode();
            else
                key->SetNormalMode();
            key->show();
        }
    }
    return false;
}

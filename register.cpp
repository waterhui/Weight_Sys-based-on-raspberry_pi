#include "register.h"
#include "ui_register.h"
#include <QDebug>
#include <QString>
#include <QGraphicsDropShadowEffect>
#include <QTimer>
#include <QDateTime>

Register::Register(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Register)
{
    ui->setupUi(this);
    QRegExp regx("[0-9]+$");//正则表达式，输入范围位0-9
    QValidator *validator = new QRegExpValidator(regx,ui->leHight);
    ui->leHight->setValidator(validator);
    ui->leHight->setMaxLength(3);
    ui->leAccount->setMaxLength(20);
    ui->lePwd->setMaxLength(20);
    ui->leName->setMaxLength(20);
    ui->rdoMale->setChecked(true);

    data_helper = new dataHelper();

    key = new keyBoard();
    key->setGeometry(0,300,1024,300);
    key->setWindowModality(Qt::ApplicationModal);
    key->hide();

    //install
    ui->leAccount->installEventFilter(this);
    ui->lePwd->installEventFilter(this);
    ui->lePwdConfrim->installEventFilter(this);
    ui->leName->installEventFilter(this);
    ui->leHight->installEventFilter(this);

    //cao hanshu
    connect(ui->btnOk,SIGNAL(clicked()),this,SLOT(toOk()));
    connect(ui->btnCancel, SIGNAL(clicked()), this, SLOT(toCancel()));

    connect(key, SIGNAL(Hide()), this, SLOT(Hide()));
    connect(key,SIGNAL(Edit(QString)),this,SLOT(Edit(QString)));

    //this->showFullScreen();
    setWindowModality(Qt::ApplicationModal); //设置模态

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(time_slot()));
    timer->start(1000);

    QPalette lcdpat = ui->lcdNumber->palette();
    /*设置颜色，整体背景颜色 颜色蓝色,此函数的第一个参数可以设置多种。如文本、按钮按钮文字、多种*/
    lcdpat.setColor(QPalette::Normal,QPalette::WindowText,Qt::blue);
    //设置当前窗口的调色板
    ui->lcdNumber->setPalette(lcdpat);
    //设置背景色
    ui->lcdNumber->display(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));

    time_slot();

    QLabel *my_label = new QLabel(this);
    my_label->setStyleSheet("background-color:rgb(50 , 204 , 194);");
    my_label->setGeometry(780, 0, this->width() - 780, this->height());
    my_label->lower();
    this->setStyleSheet("background-color:rgb(255 , 255 , 255);");


   QGraphicsDropShadowEffect *shadow_effect_1 = new QGraphicsDropShadowEffect(this);
   QGraphicsDropShadowEffect *shadow_effect_2 = new QGraphicsDropShadowEffect(this);
   QGraphicsDropShadowEffect *shadow_effect_3 = new QGraphicsDropShadowEffect(this);
   QGraphicsDropShadowEffect *shadow_effect_4 = new QGraphicsDropShadowEffect(this);
   QGraphicsDropShadowEffect *shadow_effect_5= new QGraphicsDropShadowEffect(this);
   QGraphicsDropShadowEffect *shadow_effect_6= new QGraphicsDropShadowEffect(this);
   QGraphicsDropShadowEffect *shadow_effect_7= new QGraphicsDropShadowEffect(this);
   QGraphicsDropShadowEffect *shadow_effect_8= new QGraphicsDropShadowEffect(this);
   shadow_effect_1->setOffset(5, 5);
   shadow_effect_1->setColor(Qt::gray);
   shadow_effect_1->setBlurRadius(8);
   shadow_effect_2->setOffset(5, 5);
   shadow_effect_2->setColor(Qt::gray);
   shadow_effect_2->setBlurRadius(8);
   shadow_effect_3->setOffset(5, 5);
   shadow_effect_3->setColor(Qt::gray);
   shadow_effect_3->setBlurRadius(8);
   shadow_effect_4->setOffset(5, 5);
   shadow_effect_4->setColor(Qt::gray);
   shadow_effect_4->setBlurRadius(8);
   shadow_effect_5->setOffset(5, 5);
   shadow_effect_5->setColor(Qt::gray);
   shadow_effect_5->setBlurRadius(8);
   shadow_effect_6->setOffset(5, 5);
   shadow_effect_6->setColor(Qt::gray);
   shadow_effect_6->setBlurRadius(8);
   shadow_effect_7->setOffset(5, 5);
   shadow_effect_7->setColor(Qt::gray);
   shadow_effect_7->setBlurRadius(8);
   shadow_effect_8->setOffset(5, 5);
   shadow_effect_8->setColor(Qt::gray);
   shadow_effect_8->setBlurRadius(8);

   ui->leAccount->setGraphicsEffect(shadow_effect_1);
   ui->lePwd->setGraphicsEffect(shadow_effect_2);
   ui->lePwdConfrim->setGraphicsEffect(shadow_effect_3);
   ui->leName->setGraphicsEffect(shadow_effect_4);
   ui->leHight->setGraphicsEffect(shadow_effect_5);
   ui->rdoMale->setGraphicsEffect(shadow_effect_6);
   ui->rboFeMale->setGraphicsEffect(shadow_effect_7);
   ui->gbInfo->setGraphicsEffect(shadow_effect_8);

   QLabel *second_label = new QLabel(this);
    second_label->setStyleSheet("border-image: url(:/imgIcon/appbar.social.github.octocat.png); \
                                                background-color: transparent;");

    second_label->setGeometry((my_label->width() - 76) / 2 + my_label->x(), (this->height() - 76) / 2, 76, 76);

    ui->btnCancel->setFocus();
}

Register::~Register()
{
    delete key;
    delete data_helper;
    delete ui;
}


bool Register::isFillAll()
{
    bool flag = true;
    if(ui->leAccount->text() == ""
            || ui->lePwd->text() == ""
            || ui->lePwdConfrim->text() == ""
            || ui->leName->text() == ""
            || ui->leHight->text() == "")
    {
        flag = false;
    }

    return flag;
}

void Register::time_slot()
{
    ui->lcdNumber->setDigitCount(19);
    ui->lcdNumber->setSegmentStyle(QLCDNumber::Flat);
    ui->lcdNumber->display((new QDateTime) ->currentDateTime()
                           .toString("yyyy-MM-dd hh:mm:ss"));
}

void Register::toOk()
{
    Message_Box *message_box;
    message_box = new Message_Box();
    message_box->setTitle("注意");
    message_box->setText("请填满所有信息！");
    message_box->setShowButton(false, true);
    message_box->setImg(Message_Box::Warning);
    if(!isFillAll()) {
        message_box->exec();
        return ;
    }
    if(ui->lePwd->text().trimmed() != ui->lePwdConfrim->text().trimmed()) {
        message_box->setText("两次输入的密码不一致！");
        message_box->exec();
        return ;
    }
    QString select = QString("select Id from Login where Id = '%1' ")
            .arg(ui->leAccount->text());

    if (data_helper->Select(select)) {
        message_box->setText("该账号已经存在！");
        message_box->exec();
        return;
    }

    QString info_insert = QString("insert into userInfo values('%1','%2','%3','%4') ")
            .arg(ui->leAccount->text().trimmed())
            .arg(ui->leName->text().trimmed())
            .arg(ui->rboFeMale->isChecked() ? "女" : "男")
            .arg(ui->leHight->text());

    QString login_insert = QString("insert into Login values('%1', '%2') ")
            .arg(ui->leAccount->text().trimmed())
            .arg(ui->lePwd->text().trimmed());

    bool respone_userInfo = data_helper->Insert(info_insert);
    bool respone_login = data_helper->Insert(login_insert);
    if (respone_userInfo && respone_login) {
        message_box->setText("注册成功！");
        message_box->setImg("border-image: url(:/imgIcon/appbar.people.checkbox.png);");
        message_box->exec();
    } else {
        message_box->setImg("border-image: url(:/imgIcon/appbar.people.status.png);");
        message_box->setText("注册失败！");
        QString info_delete = QString("delete * from Login where Id = '%1' ")
                .arg(ui->leAccount->text().trimmed());
        QString login_delete = QString("delete * from userInfo where Id = '%1' ")
                .arg(ui->leAccount->text().trimmed());
        if (!respone_userInfo)
            data_helper->Delete(info_delete);
        if (!respone_login)
            data_helper->Delete(login_delete);
        message_box->exec();
    }
    this->close();
}


bool Register::eventFilter(QObject *obj, QEvent *e) {
    if (obj == ui->leAccount || obj == ui->lePwd
            || obj == ui->leName || obj == ui->leHight
            || obj == ui->lePwdConfrim) {
        if (e->type() == QEvent::FocusIn) {
            select_LE = (QLineEdit *)obj;
            if (select_LE == ui->lePwd || select_LE == ui->lePwdConfrim) {
                key->SetPwdMode();
            } else {
                key->SetNormalMode();
            }
            key->setLeText(select_LE->text());
            key->show();
            return false;
        }
    } else {
        ui->btnCancel->setFocus();
    }
    return false;
}

void Register::toCancel() {
    this->close();
}

void Register::Edit(QString str) {
    if (select_LE == ui->leHight) {
        for(int i = 0; i < str.size(); i++) {
            if (str.at(i) < '0' || str.at(i) > '9') return;
        }
    }
    select_LE->setText(str);
}

void Register::Hide() {
    ui->btnCancel->setFocus();
    key->hide();
}




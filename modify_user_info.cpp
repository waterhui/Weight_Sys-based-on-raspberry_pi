#include "modify_user_info.h"
#include "ui_modify_user_info.h"

#include <QDebug>
#include <QEvent>
#include <QGraphicsDropShadowEffect>

modify_user_info::modify_user_info(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::modify_user_info)
{
    ui->setupUi(this);

    QRegExp regx("[0-9]+$");//正则表达式，输入范围位0-9
    QValidator *validator = new QRegExpValidator(regx,ui->leHeight);
    ui->leHeight->setValidator(validator);
    ui->leHeight->setMaxLength(3);
    ui->leName->setMaxLength(20);
    ui->lePwd->setMaxLength(20);
    ui->leConfirmPwd->setMaxLength(20);

    this->setWindowFlags(this->windowFlags() &~ Qt::WindowMinMaxButtonsHint);
    setFixedSize(this->width(),this->height()); // 禁止改变窗口大小。
    this->setWindowFlags((Qt::FramelessWindowHint)); //去掉窗体边框
    setWindowModality(Qt::ApplicationModal); //设置模态

    ui->leName->installEventFilter(this);
    ui->lePwd->installEventFilter(this);
    ui->leHeight->installEventFilter(this);
    ui->leConfirmPwd->installEventFilter(this);

    key = new keyBoard();
    key->setGeometry(0, 300, 1024, 300);
    key->setWindowModality(Qt::ApplicationModal);
    //槽函数
    connect(ui->btn_cancel, SIGNAL(clicked()), this, SLOT(to_cancel()));
    connect(ui->btn_ok, SIGNAL(clicked()), this, SLOT(to_ok()));
    connect(ui->btn_close, SIGNAL(clicked()), this, SLOT(close()));

    connect(key,  SIGNAL(Hide()), this, SLOT(to_hide()));
    connect(key, SIGNAL(Edit(QString)), this, SLOT(to_edit(QString)));

    ui->btn_cancel->setFocus();

    this->setFixedSize(this->width(), this->height());
    this->setGeometry((1024 - this->width())/ 2, 50, this->width(), this->height());


    this->setAttribute(Qt::WA_TranslucentBackground);
    setAutoFillBackground(false);

    QLabel *my_label = new QLabel(this);
    my_label->setGeometry(10, 10, this->width() - 20, 50);
    my_label->setStyleSheet("background-color:rgb(50 , 204 , 194);");
    my_label->lower();

   QGraphicsDropShadowEffect *shadow_effect_1 = new QGraphicsDropShadowEffect(this);
   QGraphicsDropShadowEffect *shadow_effect_2 = new QGraphicsDropShadowEffect(this);
   QGraphicsDropShadowEffect *shadow_effect_3 = new QGraphicsDropShadowEffect(this);
   QGraphicsDropShadowEffect *shadow_effect_4 = new QGraphicsDropShadowEffect(this);
   QGraphicsDropShadowEffect *shadow_effect_5 = new QGraphicsDropShadowEffect(this);
   QGraphicsDropShadowEffect *shadow_effect_6 = new QGraphicsDropShadowEffect(this);
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
   ui->leName->setGraphicsEffect(shadow_effect_1);
   ui->lePwd->setGraphicsEffect(shadow_effect_2);
   ui->leConfirmPwd->setGraphicsEffect(shadow_effect_3);
   ui->leHeight->setGraphicsEffect(shadow_effect_4);
   ui->rdoFemale->setGraphicsEffect(shadow_effect_5);
   ui->rdoMale->setGraphicsEffect(shadow_effect_6);


   this->setAttribute(Qt::WA_DeleteOnClose);
}

modify_user_info::~modify_user_info()
{
    if (key != NULL) {
        delete key;
    }
    if (data_helper != NULL) {
        delete data_helper;
    }
    delete ui;
}

void modify_user_info::to_cancel()
{
    this->close();
}

//保存修改
void modify_user_info::to_ok()
{
    QString sex;
    Message_Box *message_box;
    message_box = new Message_Box();
    message_box->setImg(Message_Box::Warning);
    message_box->setTitle("注意");

    if (!_isFillAll()) {
        message_box->setText("请将信息填满！");
        message_box->show();
        return;
    }

    if (!_isMatchPwd()) {
        message_box->setText("两次输入的密码不一致！");
        message_box->show();
        return;
    }
    message_box->setTitle("询问");
    message_box->setImg(Message_Box::Question);
    message_box->setText("是否保存？");


    if (message_box->exec() == QDialog::Rejected) {
        this->close();
        return;
    }

    Message_Box message_box_2;
    message_box_2.setImg(Message_Box::Warning);
    message_box_2.setTitle("注意");
    message_box_2.setText("修改失败！");
    if (ui->rdoMale->isChecked()) {
        sex = "男";
    } else {
        sex = "女";
    }
    data_helper = new dataHelper();

    //更新登录表
    QString update_login = QString("update Login set Pwd = '%1' where 	Id = '%2' ")
            .arg(ui->lePwd->text().trimmed())
            .arg(mem_info.getId());
    if(!data_helper->Update(update_login))
            message_box_2.exec();

    //更新用户信息表
    QString update_info = QString("update userInfo set 		\
                             Name = '%1' , Sex = '%2' , Height = '%3'  \
                             where Id = '%4' ")
            .arg(ui->leName->text().trimmed())
            .arg(sex)
            .arg(ui->leHeight->text().trimmed())
            .arg(mem_info.getId());
    if (!data_helper->Update(update_info))
            message_box_2.exec();

    //设置mem_info:通过QLineEdit
    _setUserInfo();
    //通知member_cente更新界面空间
    emit(sucess(mem_info));
    this->close();
}


//过滤器
bool modify_user_info::eventFilter(QObject *o, QEvent *e)
{
    if(o == ui->leName || o == ui->lePwd || o == ui->leHeight || o == ui->leConfirmPwd) {
        if (e->type() == QEvent::FocusIn) {
            select_LE = (QLineEdit *)o;
            if (select_LE == ui->lePwd || select_LE == ui->leConfirmPwd) {
                key->SetPwdMode();
            } else {
                key->SetNormalMode();
            }
            key->setLeText(select_LE->text());
            key->show();
            return false;
        }
    } else {
            key->hide();
            ui->btn_ok->setFocus();
    }

    return false;
}

//隐藏键盘
void modify_user_info::to_hide()
{
    key->hide();
    ui->btn_ok->setFocus();
}

//通过键盘编辑文本框
void modify_user_info::to_edit(QString str)
{
    if (select_LE == ui->leHeight) {
        for (int i = 0; i < str.size(); i++) {
            if (str.at(i) < '0'  || str.at(i) > '9') return;
        }
    }
    select_LE->setText(str);
}

//设置mem_info:通过直接传进来的mem_info
void modify_user_info::setUserInfo(Member_Info mem_info)
{
    this->mem_info = mem_info;
}

//设置mem_info:通过QLineEdit
void modify_user_info::_setUserInfo()
{
    mem_info.setName(ui->leName->text().trimmed());
    mem_info.setPwd(ui->lePwd->text().trimmed());
    mem_info.setHeight(ui->leHeight->text().trimmed());

    if(ui->rdoMale->isChecked())
        mem_info.setSex("男");
   else
        mem_info.setSex("女");
}

//设置界面信息
void modify_user_info::setInterfaceInfo()
{
    ui->leName->setText(mem_info.getName());
    ui->lePwd->setText(mem_info.getPwd());
    ui->leConfirmPwd->setText(ui->lePwd->text());
    ui->leHeight->setText(mem_info.getHeight());

    if(mem_info.getSex() == "男")
        ui->rdoMale->setChecked(true);
    else
        ui->rdoFemale->setChecked(true);
}

//判断所有文本框是否填满
bool modify_user_info::_isFillAll()
{
    bool flag = true;
    if (ui->leName->text().trimmed() == "" || ui->lePwd->text().trimmed() == " "
            || ui->leConfirmPwd->text().trimmed() == "" || ui->leHeight->text().trimmed() == "")
        flag = false;

    return flag;
}

//判断两次输入的密码是否一致
bool modify_user_info::_isMatchPwd()
{
    bool flag = true;
    if(ui->lePwd->text().trimmed() != ui->leConfirmPwd->text().trimmed())
        flag = false;
    return flag;
}




//鼠标按下事件
void modify_user_info::mousePressEvent(QMouseEvent *event)
{
    mousePoint = event->pos();    //鼠标相对于窗体的位置
    isMousePressed = true;        //鼠标按下
    event->accept();
}

//窗体拖动事件
void modify_user_info::mouseMoveEvent(QMouseEvent *event)
{
    //若鼠标左键被按下
    if(isMousePressed == true)
    {
        //鼠标相对于屏幕的位置
        QPoint curMousePoint = event->globalPos() - mousePoint;
        //移动主窗体位置
        move(curMousePoint);
    }
    event->accept();
}

//鼠标释放事件
void modify_user_info::mouseReleaseEvent(QMouseEvent *event)
{
    //鼠标未按下
    isMousePressed = false;
    event->accept();
}

void modify_user_info::paintEvent(QPaintEvent *event)
{

    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    path.addRect(0, 0, this->width(), this->height());
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.fillPath(path, QBrush(Qt::white));

    QColor color(0, 0, 0, 50);
    for(int i=0; i<10; i++)
    {
        QPainterPath path;
        path.setFillRule(Qt::WindingFill);
        path.addRect(10-i, 10-i, this->width()-(10-i)*2, this->height()-(10-i)*2);
        color.setAlpha(220 - qSqrt(i)*50);
        painter.setPen(color);
        painter.drawPath(path);
    }
    QWidget::paintEvent(event);
}

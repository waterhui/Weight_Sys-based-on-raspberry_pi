#include "admin_modify_pwd.h"
#include "ui_admin_modify_pwd.h"

#include <QDebug>
#include <QGraphicsDropShadowEffect>

Admin_Modify_Pwd::Admin_Modify_Pwd(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Admin_Modify_Pwd)
{
    ui->setupUi(this);
    ui->lePwd->setMaxLength(20);
    ui->lePwdConfirm->setMaxLength(20);

    connect(ui->btnCancel, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->btnOk, SIGNAL(clicked()), this, SLOT(to_ok()));
    connect(ui->btn_close, SIGNAL(clicked()), this, SLOT(close()));

    this->setWindowModality(Qt::ApplicationModal);

    //设置窗口禁止最大化和最小化
    this->setWindowFlags(this->windowFlags() &~ Qt::WindowMinMaxButtonsHint);

    data_helper = new dataHelper();
    key = new keyBoard();
    key->setWindowModality(Qt::ApplicationModal);
    key->SetPwdMode();

    connect(key, SIGNAL(Edit(QString)), this, SLOT(to_edit(QString)));
    connect(key, SIGNAL(Hide()), this, SLOT(to_reshow()));

    ui->lePwd->installEventFilter(this);
    ui->lePwdConfirm->installEventFilter(this);


    this->setFixedSize(this->width(), this->height());
    this->setGeometry((1024 - this->width()) / 2, 100, this->width(), this->height());

    this->setWindowFlags((Qt::FramelessWindowHint)); //去掉窗体边框
    this->setAttribute(Qt::WA_TranslucentBackground);
    setAutoFillBackground(false);

    QLabel *my_label = new QLabel(this);
    my_label->setGeometry(10, 10, this->width() - 20, 50);
    my_label->setStyleSheet("background-color:rgb(50 , 204 , 194);");
    my_label->lower();


   QGraphicsDropShadowEffect *shadow_effect_1 = new QGraphicsDropShadowEffect(this);
   QGraphicsDropShadowEffect *shadow_effect_2 = new QGraphicsDropShadowEffect(this);
   shadow_effect_1->setOffset(5, 5);
   shadow_effect_1->setColor(Qt::gray);
   shadow_effect_1->setBlurRadius(8);
   shadow_effect_2->setOffset(5, 5);
   shadow_effect_2->setColor(Qt::gray);
   shadow_effect_2->setBlurRadius(8);

   ui->lePwd->setGraphicsEffect(shadow_effect_1);
   ui->lePwdConfirm->setGraphicsEffect(shadow_effect_2);
   ui->btnCancel->setFocus();
   this->setAttribute(Qt::WA_DeleteOnClose);
}

Admin_Modify_Pwd::~Admin_Modify_Pwd()
{
    delete data_helper;
    delete key;
    delete ui;
}


/*****************slots******************/

void Admin_Modify_Pwd::to_ok()
{
    Message_Box *message_box = new Message_Box();
    message_box->setTitle("注意");
    message_box->setImg(Message_Box::Warning);
    message_box->setShowButton(false, true);
    if ( !_isFillAll()) {
        message_box->setText("密码不能为空！");
        message_box->exec();
        return;
    }

    if (!_isConsistent()) {
        message_box->setText("两次输入的密码不一致！");
        message_box->exec();
        return;
    }

    message_box->setTitle("询问");
    message_box->setText("是否保存修改？");
    message_box->setImg(Message_Box::Question);
    message_box->setShowButton(true, true);
    admin_info.setPwd(ui->lePwd->text().trimmed());
    if (message_box->exec() == QDialog::Rejected) {
        this->close();
        return;
    }
    QString sql_update = QString("update Login set Pwd = '%1' where Id = '%2' " )
            .arg(admin_info.getPwd())
            .arg(admin_info.getId());
    data_helper->Update(sql_update);
    emit success(admin_info);
    this->close();
}

void Admin_Modify_Pwd::to_edit(QString str)
{
    this->select_edit->setText(str);
}

void Admin_Modify_Pwd::to_reshow()
{
    key->hide();
    ui->btnCancel->setFocus();
}


/***************************************/


void Admin_Modify_Pwd::setAdminInfo(Member_Info member_info)
{
    this->admin_info = member_info;
    ui->lePwd->setText(admin_info.getPwd());
    ui->lePwdConfirm->setText(admin_info.getPwd());
}

bool Admin_Modify_Pwd::_isFillAll()
{
    bool flag = false;
    if(ui->lePwd->text().trimmed() != "" && ui->lePwdConfirm->text().trimmed() != "") {
        flag = true;
    }
    return flag;
}

bool Admin_Modify_Pwd::_isConsistent()
{
    bool flag = false;
    if (ui->lePwd->text().trimmed() == ui->lePwdConfirm->text().trimmed()) {
        flag = true;
    }
    return flag;
}


bool Admin_Modify_Pwd::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->lePwd || obj == ui->lePwdConfirm) {
            if (event->type() == QEvent::FocusIn) {
                select_edit = (QLineEdit *)obj;
                key->setLeText(select_edit->text().trimmed());
                key->show();
            }
    }

    return false;
}

//鼠标按下事件
void Admin_Modify_Pwd::mousePressEvent(QMouseEvent *event)
{
    mousePoint = event->pos();    //鼠标相对于窗体的位置
    isMousePressed = true;        //鼠标按下
    event->accept();
}

//窗体拖动事件
void Admin_Modify_Pwd::mouseMoveEvent(QMouseEvent *event)
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
void Admin_Modify_Pwd::mouseReleaseEvent(QMouseEvent *event)
{
    //鼠标未按下
    isMousePressed = false;
    event->accept();
}

void Admin_Modify_Pwd::paintEvent(QPaintEvent *event)
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

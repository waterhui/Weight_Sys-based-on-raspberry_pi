#include "time_setting.h"
#include "ui_time_setting.h"

#include <QDebug>
#include <QDateTime>

#include <QProcess>
#include <QMessageBox>
#include <QGraphicsDropShadowEffect>


Time_Setting::Time_Setting(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Time_Setting)
{
    ui->setupUi(this);

    connect(ui->btnOk, SIGNAL(clicked()), this, SLOT(to_ok()));
    connect(ui->btnCancel, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->btn_close, SIGNAL(clicked()), this, SLOT(close()));

    ui->dateTimeEdit->setDateTime((new QDateTime())->currentDateTime());

    setFixedSize(this->width(), this->height());

    setWindowModality(Qt::ApplicationModal); //设置模态

    //设置窗口禁止最大化和最小化
    this->setWindowFlags(this->windowFlags() &~ Qt::WindowMinMaxButtonsHint);

    this->setFixedSize(this->width(),  this->height());
    this->setGeometry((1024 - this->width()) / 2, 100, this->width(), this->height());


    this->setWindowFlags((Qt::FramelessWindowHint)); //去掉窗体边框
    this->setAttribute(Qt::WA_TranslucentBackground);
    setAutoFillBackground(false);

    QLabel *my_label = new QLabel(this);
    my_label->setGeometry(10, 10, this->width() - 20, 50);
    my_label->setStyleSheet("background-color:rgb(50 , 204 , 194);");
    my_label->lower();

   QGraphicsDropShadowEffect *shadow_effect_1 = new QGraphicsDropShadowEffect(this);
   shadow_effect_1->setOffset(5, 5);
   shadow_effect_1->setColor(Qt::gray);
   shadow_effect_1->setBlurRadius(8);

   ui->dateTimeEdit->setGraphicsEffect(shadow_effect_1);

    this->setAttribute(Qt::WA_DeleteOnClose);
}

Time_Setting::~Time_Setting()
{
    delete ui;
}


void Time_Setting::to_ok()
{
    Message_Box *message_box = new Message_Box();
    message_box->setTitle("询问");
    message_box->setText("是否保存修改？");
    message_box->setImg(Message_Box::Question);
    message_box->setShowButton(true, true);

    if (message_box->exec() == QDialog::Rejected) {
        this->close();
    }
    QString date  = ui->dateTimeEdit->text();
    QString last = date.replace("/", "-").append(":00");

    QString command = "sudo date --s=\"" + last + "\"";

    QProcess::execute(command);
    this->close();
    //QProcess::execute("sync");
}


//鼠标按下事件
void Time_Setting::mousePressEvent(QMouseEvent *event)
{
    mousePoint = event->pos();    //鼠标相对于窗体的位置
    isMousePressed = true;        //鼠标按下
    event->accept();
}

//窗体拖动事件
void Time_Setting::mouseMoveEvent(QMouseEvent *event)
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
void Time_Setting::mouseReleaseEvent(QMouseEvent *event)
{
    //鼠标未按下
    isMousePressed = false;
    event->accept();
}

void Time_Setting::paintEvent(QPaintEvent *event)
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

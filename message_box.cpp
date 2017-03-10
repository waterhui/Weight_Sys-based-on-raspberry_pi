#include "message_box.h"
#include "ui_message_box.h"


Message_Box::Message_Box(QDialog *parent) :
    QDialog(parent),
    ui(new Ui::Message_Box)
{
    ui->setupUi(this);

    setAutoFillBackground(false);
    this->setWindowFlags((Qt::FramelessWindowHint)); //去掉窗体边框
    setWindowModality(Qt::ApplicationModal); //设置模态

    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setFixedSize(this->width(), this->height());

    connect(ui->btn_close, SIGNAL(clicked()), this, SLOT(reject()));
    connect(ui->btn_ok, SIGNAL(clicked()), this, SLOT(accept()));
    connect(ui->btn_cancel, SIGNAL(clicked()), this, SLOT(reject()));

    QLabel *my_label = new QLabel(this);
    my_label->setGeometry(10, 10, this->width() - 20, 50);
    my_label->setStyleSheet("background-color:rgb(50 , 204 , 194);");
    my_label->lower();

    this->setGeometry((1024 - this->width()) / 2, (600 - this->height()) / 2,this->width(), this->height());

    //QWSServer::setBackground(QColor(0,0,0,0));
    this->setAttribute(Qt::WA_DeleteOnClose);
}

/********************static******************/
int Message_Box::Warning = 0;
int Message_Box::Question = 1;

QString Message_Box::str_warning = "border-image: url(:/imgIcon/appbar.transit.hazard.png);";
QString Message_Box::str_question = "border-image: url(:/imgIcon/appbar.information.circle.png);";
/******************************************/

Message_Box::~Message_Box()
{
    delete ui;
}

void Message_Box::setTitle(QString title)
{
    ui->lblTitle->setText(title);
}

void Message_Box::setText(QString str)
{
    ui->lblInfo->setText(str);
}

void Message_Box::setImg(int command)
{
    if (command == 0)
        ui->lblImg->setStyleSheet(str_warning);
    else if (command == 1)
        ui->lblImg->setStyleSheet(str_question);
}

void Message_Box::setImg(QString command)
{
    ui->lblImg->setStyleSheet(command);
}

void Message_Box::setShowButton(bool flag_ok, bool flag_cancel)
{
    if (!flag_ok)
        ui->btn_ok->hide();
    else
        ui->btn_ok->show();
    if (!flag_cancel)
        ui->btn_cancel->hide();
    else
        ui->btn_cancel->show();
}

/*********************slot*********************/

/*********************************************/

//鼠标按下事件
void Message_Box::mousePressEvent(QMouseEvent *event)
{
    mousePoint = event->pos();    //鼠标相对于窗体的位置
    isMousePressed = true;        //鼠标按下
    event->accept();
}

//窗体拖动事件
void Message_Box::mouseMoveEvent(QMouseEvent *event)
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
void Message_Box::mouseReleaseEvent(QMouseEvent *event)
{
    //鼠标未按下
    isMousePressed = false;
    event->accept();
}

void Message_Box::paintEvent(QPaintEvent *event)
{
    QPalette pal = palette();
    pal.setColor(QPalette::Window, QColor(0x00,0xff,0x00,0x00));
    setPalette(pal);

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

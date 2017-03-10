#include "modify_comment.h"
#include "ui_modify_comment.h"

#include <QLabel>
#include <QGraphicsDropShadowEffect>

Modify_Comment::Modify_Comment(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Modify_Comment)
{
    ui->setupUi(this);

    ui->btnCancel->setFocus();

    ui->txtEditComment->installEventFilter(this);

    key = new keyBoard();
    data_helper = new dataHelper();

    connect(ui->btnCancel, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->btnOk, SIGNAL(clicked()), this, SLOT(to_ok()));
    connect(ui->btn_close, SIGNAL(clicked()), this, SLOT(close()));

    connect(key, SIGNAL(Hide()), this, SLOT(to_hide()));
    connect(key, SIGNAL(Edit(QString)), this, SLOT(to_edit(QString)));

    this->setWindowModality(Qt::ApplicationModal);

    key->setWindowModality(Qt::ApplicationModal);

    this->setFixedSize(this->width(), this->height());
    this->setGeometry((1024 - this->width()) / 2, 50, this->width(), this->height());


    this->setWindowFlags((Qt::FramelessWindowHint)); //去掉窗体边框
    this->setAttribute(Qt::WA_TranslucentBackground);
    setAutoFillBackground(false);

    ui->btnCancel->setFocus();

    QLabel *my_label = new QLabel(this);
    my_label->setGeometry(10, 10, this->width() - 20, 50);
    my_label->setStyleSheet("background-color:rgb(50 , 204 , 194);");
    my_label->lower();


   QGraphicsDropShadowEffect *shadow_effect_1 = new QGraphicsDropShadowEffect(this);
   shadow_effect_1->setOffset(5, 5);
   shadow_effect_1->setColor(Qt::gray);
   shadow_effect_1->setBlurRadius(8);

   ui->txtEditComment->setGraphicsEffect(shadow_effect_1);
   this->setAttribute(Qt::WA_DeleteOnClose);
}

Modify_Comment::~Modify_Comment()
{
    delete key;
    delete data_helper;
    delete ui;
}

/**********************slots*********************/

void Modify_Comment::to_hide()
{
    key->hide();
    ui->btnCancel->setFocus();
    this->show();
}

void Modify_Comment::to_edit(QString str)
{
    ui->txtEditComment->setPlainText(str);
}

void Modify_Comment::to_ok()
{
    Message_Box *message_box = new Message_Box();
    message_box->setTitle("询问");
    message_box->setText("是否保存？");
    message_box->setImg(Message_Box::Question);
    message_box->setShowButton(true, true);

    if (message_box->exec() == QDialog::Rejected) {
        this->close();
        return;
    }
    comment_info.setRemark(ui->txtEditComment->toPlainText().trimmed());
    QString sql_update = QString("update Comment set Remark = '%1' where Status = '%2' ")
            .arg(comment_info.getRemark())
            .arg(comment_info.getStatus());
    data_helper->Update(sql_update);
    emit to_success(comment_info);
    this->close();
}

/***********************************************/


void Modify_Comment::setCommentInfo(Comment_Info comment_info)
{
    this->comment_info = comment_info;
}

void Modify_Comment::initInterface()
{
    ui->txtEditComment->setText(comment_info.getRemark());
}

bool Modify_Comment::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->txtEditComment) {
        if (event->type() == QEvent::FocusIn) {
            key->setLeText(ui->txtEditComment->toPlainText());
            key->show();
        }
    }

    return false;
}


//鼠标按下事件
void Modify_Comment::mousePressEvent(QMouseEvent *event)
{
    mousePoint = event->pos();    //鼠标相对于窗体的位置
    isMousePressed = true;        	 //鼠标按下
    event->accept();
}

//窗体拖动事件
void Modify_Comment::mouseMoveEvent(QMouseEvent *event)
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
void Modify_Comment::mouseReleaseEvent(QMouseEvent *event)
{
    //鼠标未按下
    isMousePressed = false;
    event->accept();
}

void Modify_Comment::paintEvent(QPaintEvent *event)
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

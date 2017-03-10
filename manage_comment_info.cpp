#include "manage_comment_info.h"
#include "ui_manage_comment_info.h"

#include <QAbstractItemModel>
#include <QGraphicsDropShadowEffect>
#include <QDebug>

Manage_Comment_Info::Manage_Comment_Info(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Manage_Comment_Info)
{
    ui->setupUi(this);

    connect(ui->btnExit, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->btnModify, SIGNAL(clicked()), this, SLOT(to_modify()));
    connect(ui->tableComment,SIGNAL(pressed(QModelIndex)), this, SLOT(to_show_detail(QModelIndex)));
    connect(ui->btn_close, SIGNAL(clicked()), this, SLOT(close()));

    data_helper = new dataHelper();

    initInterface();


    this->setWindowFlags((Qt::FramelessWindowHint)); //去掉窗体边框
    this->setAttribute(Qt::WA_TranslucentBackground);
    setAutoFillBackground(false);

    QLabel *my_label = new QLabel(this);
    my_label->setGeometry(10, 10, this->width() - 20, 50);
    my_label->setStyleSheet("background-color:rgb(50 , 204 , 194);");
    my_label->lower();

    this->setFixedSize(this->width(), this->height());
    this->setGeometry((1024 - this->width()) / 2, 10 , this->width(), this->height());


   QGraphicsDropShadowEffect *shadow_effect_1 = new QGraphicsDropShadowEffect(this);
   QGraphicsDropShadowEffect *shadow_effect_2 = new QGraphicsDropShadowEffect(this);
   shadow_effect_1->setOffset(5, 5);
   shadow_effect_1->setColor(Qt::gray);
   shadow_effect_1->setBlurRadius(8);
   shadow_effect_2->setOffset(5, 5);
   shadow_effect_2->setColor(Qt::gray);
   shadow_effect_2->setBlurRadius(8);

   ui->tableComment->setGraphicsEffect(shadow_effect_1);
   ui->txtBrowserComment->setGraphicsEffect(shadow_effect_2);
   this->setAttribute(Qt::WA_DeleteOnClose);
}

Manage_Comment_Info::~Manage_Comment_Info()
{
    delete data_helper;
    delete ui;
}

void Manage_Comment_Info::initInterface()
{
    _setCommentInfo();
    QStandardItemModel  *model = new QStandardItemModel();
    //用户名Id,密码(Pwd),姓名,性别,身高
    model->setColumnCount(2);
    model->setHeaderData(0, Qt::Horizontal, "体重状况");
    model->setHeaderData(1, Qt::Horizontal, "评语");

    for(int i = 0; i < comment_info.size(); i++)
    {
        model->setItem(i, 0, new QStandardItem(comment_info.at(i).getStatus()));
        model->setItem(i, 1, new QStandardItem(comment_info.at(i).getRemark()));

        model->item(i,0)->setTextAlignment(Qt::AlignCenter);
        model->item(i,1)->setTextAlignment(Qt::AlignCenter);
    }

    ui->tableComment->setModel(model);

    //设置列宽可伸缩
    ui->tableComment->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);

    //设置窗口禁止最大化和最小化
    this->setWindowFlags(this->windowFlags() &~ Qt::WindowMinMaxButtonsHint);

    //使tabview内的内容无法编辑
    ui->tableComment->setEditTriggers ( QAbstractItemView::NoEditTriggers );

    //选中一整行
    ui->tableComment->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
    //只能选中单行
    ui->tableComment->setSelectionMode(QAbstractItemView::SingleSelection);

    setWindowModality(Qt::ApplicationModal); //设置模态

    ui->btnExit->setFocus();

}

//从数据库中获得评论信息
void Manage_Comment_Info::_setCommentInfo()
{
    QString sql_select = QString("select * from Comment");
    data_helper->Select(sql_select, &comment_info);
}


/********************slots*********************/
void Manage_Comment_Info::to_show_detail(QModelIndex index)
{
    QAbstractItemModel *model = ui->tableComment->model();
    QStandardItem *item = ((QStandardItemModel *) model)->item(index.row(), 1);
    ui->txtBrowserComment->setText(item->text());
}

void Manage_Comment_Info::to_modify()
{
    int table_index = ui->tableComment->currentIndex().row();
    if (table_index < 0) {
        Message_Box *message_box = new Message_Box();
        message_box->setTitle("注意");
        message_box->setText("未选择任何评语！");
        message_box->setImg(Message_Box::Warning);
        message_box->setShowButton(false, true);
        message_box->exec();
        return;
    }

    modify_comment = new Modify_Comment();
    connect(modify_comment, SIGNAL(to_success(Comment_Info)), this, SLOT(to_reshow(Comment_Info)));
    modify_comment->setCommentInfo(comment_info.at(ui->tableComment->currentIndex().row()));
    modify_comment->initInterface();
    modify_comment->show();
}

void Manage_Comment_Info::to_reshow(Comment_Info comment_info)
{
    int table_index = ui->tableComment->currentIndex().row();
    QAbstractItemModel *model = ui->tableComment->model();
    QStandardItem *item = ((QStandardItemModel*)model)->item(table_index, 1);
    item->setText(comment_info.getRemark());

    (this->comment_info.operator [](table_index)).setRemark(comment_info.getRemark());
    ui->txtBrowserComment->setText(comment_info.getRemark());

    ui->tableComment->repaint();
}

/*********************************************/



//鼠标按下事件
void Manage_Comment_Info::mousePressEvent(QMouseEvent *event)
{
    mousePoint = event->pos();    //鼠标相对于窗体的位置
    isMousePressed = true;        //鼠标按下
    event->accept();
}

//窗体拖动事件
void Manage_Comment_Info::mouseMoveEvent(QMouseEvent *event)
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
void Manage_Comment_Info::mouseReleaseEvent(QMouseEvent *event)
{
    //鼠标未按下
    isMousePressed = false;
    event->accept();
}

void Manage_Comment_Info::paintEvent(QPaintEvent *event)
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

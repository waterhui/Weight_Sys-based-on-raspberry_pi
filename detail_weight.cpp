#include "detail_weight.h"
#include "ui_detail_weight.h"

#include <QStandardItemModel>
#include <QDateTime>
#include <QLabel>
#include <QPainterPath>
#include <QPainter>
#include <QtMath>
#include <QGraphicsDropShadowEffect>
#include <QDebug>

Detail_Weight::Detail_Weight(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Detail_Weight)
{
    ui->setupUi(this);
    //槽函数
    connect(ui->btn_exit, SIGNAL(clicked()), this, SLOT(to_Exit()));
    connect(ui->btnDelete, SIGNAL(clicked()), this, SLOT(to_delete()));
    connect(ui->btnDeleteAll, SIGNAL(clicked()), this, SLOT(to_delete_all()));
    connect(ui->btn_close, SIGNAL(clicked()), this, SLOT(close()));

    data_helper = new dataHelper();
    ui->btnDelete->hide();
    ui->btnDeleteAll->hide();

    setAutoFillBackground(false);
    this->setAttribute(Qt::WA_TranslucentBackground);
    ui->lblHead->lower();
    ui->lblHead->setGeometry(10, 10, this->width() - 20, 50);

    this->setWindowFlags((Qt::FramelessWindowHint)); //去掉窗体边框
    this->setFixedSize(this->width(), this->height());
    this->setGeometry((1024 - this->width()) / 2, 50, this->width(), this->height());


   QGraphicsDropShadowEffect *shadow_effect_1 = new QGraphicsDropShadowEffect(this);
   shadow_effect_1->setOffset(5, 5);
   shadow_effect_1->setColor(Qt::gray);
   shadow_effect_1->setBlurRadius(8);

   ui->tableWeight->setGraphicsEffect(shadow_effect_1);

   this->setAttribute(Qt::WA_DeleteOnClose);
}

Detail_Weight::~Detail_Weight()
{
    if (isAdminMode)
        delete weight_info;
    delete data_helper;
    delete ui;
}

/********************slot************************/
void Detail_Weight::to_Exit()
{
    this->close();
}

void Detail_Weight::to_delete()
{
    if (!_isSelectAnyWeight())
        return;

    Message_Box *message_box = new Message_Box();
    message_box->setTitle("询问");
    message_box->setText("是否删除？");
    message_box->setImg(Message_Box::Question);
    message_box->setShowButton(true, true);
    if (message_box->exec() == QDialog::Rejected) {
        return;
    }

    int table_id = ui->tableWeight->currentIndex().row();
    QString sql_delete = QString("delete from WeightInfo where Id = '%1' and Time = '%2' ")
            .arg(weight_info->at(table_id).getId())
            .arg(weight_info->at(table_id).getTime());
    data_helper->Delete(sql_delete);

    weight_info->removeAt(table_id);

    _clearTableViewData();
    _setTableViewData();
}

void Detail_Weight::to_delete_all()
{
    Message_Box *message_box = new Message_Box();
    if(weight_info->size() == 0) {
        message_box->setTitle("注意");
        message_box->setText("该用户无任何体重信息！");
        message_box->setImg(Message_Box::Warning);
        message_box->setShowButton(false, true);
        message_box->exec();
        return;
    }

    message_box->setTitle("询问");
    message_box->setText("是否删除所有体重信息？");
    message_box->setImg(Message_Box::Question);
    message_box->setShowButton(true, true);
    if (message_box->exec() == QDialog::Rejected) {
        return;
    }

    QString sql_delete_all = QString("delete from WeightInfo where Id = '%1' ")
            .arg(weight_info->at(0).getId());

    data_helper->Delete(sql_delete_all);
    this->_clearTableViewData();
    weight_info->clear();
    this->_setTableViewData();
    ui->tableWeight->repaint();
}

/************************************************/


void Detail_Weight::getDetailWeight(QList<Weight_Info> * weight_info)
{
    this->weight_info = weight_info;
}

//初始化界面
void Detail_Weight::initInterface()
{
    _setTableViewData();

    //设置列宽可伸缩
    ui->tableWeight->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);

    //设置窗口禁止最大化和最小化
    this->setWindowFlags(this->windowFlags() &~ Qt::WindowMinMaxButtonsHint);

    //使tabview内的内容无法编辑
    ui->tableWeight->setEditTriggers ( QAbstractItemView::NoEditTriggers );

    //选中一整行
    ui->tableWeight->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);

    //只能选中单行
    ui->tableWeight->setSelectionMode(QAbstractItemView::SingleSelection);

    setWindowModality(Qt::ApplicationModal); //设置模态

    ui->btn_exit->setFocus();
}

void Detail_Weight::_setTableViewData()
{
    QStandardItemModel  *model = new QStandardItemModel();
    //体重,体质比,具体肥胖程度,日期
    model->setColumnCount(4);
    model->setHeaderData(0, Qt::Horizontal, "体重(kg)");
    model->setHeaderData(1, Qt::Horizontal, "体质比(BMI)");
    model->setHeaderData(2, Qt::Horizontal, "肥胖程度");
    model->setHeaderData(3, Qt::Horizontal, "日期");

    for(int i = 0; i < weight_info->size(); i++)
    {
        model->setItem(i, 0, new QStandardItem(weight_info->at(i).getWeight()));
        model->setItem(i, 1, new QStandardItem(weight_info->at(i).getBMI()));
        model->setItem(i, 2, new QStandardItem(weight_info->at(i).getStatus()));
        model->setItem(i, 3, new QStandardItem(weight_info->at(i).getTime()));

        model->item(i,0)->setTextAlignment(Qt::AlignCenter);
        model->item(i,1)->setTextAlignment(Qt::AlignCenter);
        model->item(i,2)->setTextAlignment(Qt::AlignCenter);
        model->item(i,3)->setTextAlignment(Qt::AlignCenter);
    }

    ui->tableWeight->setModel(model);
}

//给用户使用
void Detail_Weight::setUserWeight(QString Id)
{
    QString sql_select = QString("select * from WeightInfo where Id = '%1' order by Time")
            .arg(Id);
    data_helper->Select(sql_select, weight_info);
}

//设置为管理员模式
void Detail_Weight::setAdminMode()
{
    ui->btnDelete->show();
    ui->btnDeleteAll->show();
    weight_info = new QList<Weight_Info>();
    isAdminMode = true;
    //ui->tableWeight->setGeometry(20, 30, 491, 301);
}

void Detail_Weight::_clearTableViewData()
{
    QAbstractItemModel *model  = ui->tableWeight->model();
    delete model;
}

bool Detail_Weight::_isSelectAnyWeight()
{
    int table_id = ui->tableWeight->currentIndex().row();
    if (table_id < 0) {
        Message_Box *message_box = new Message_Box();
        message_box->setTitle("注意");
        message_box->setText("未选择任何体重信息！");
        message_box->setImg(Message_Box::Warning);
        message_box->setShowButton(false, true);
        message_box->exec();
        return false;
    }
    return true;
}

//鼠标按下事件
void Detail_Weight::mousePressEvent(QMouseEvent *event)
{
    mousePoint = event->pos();    //鼠标相对于窗体的位置
    isMousePressed = true;        //鼠标按下
    event->accept();
}

//窗体拖动事件
void Detail_Weight::mouseMoveEvent(QMouseEvent *event)
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
void Detail_Weight::mouseReleaseEvent(QMouseEvent *event)
{
    //鼠标未按下
    isMousePressed = false;
    event->accept();
}

void Detail_Weight::paintEvent(QPaintEvent *event)
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

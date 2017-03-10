#include "member_center.h"
#include "ui_member_center.h"

#include <QGraphicsDropShadowEffect>


Member_Center::Member_Center(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Member_Center)
{
    ui->setupUi(this);



    connect(ui->btn_change, SIGNAL(clicked()), this, SLOT(to_change()));
    connect(ui->btnModify, SIGNAL(clicked()), this, SLOT(to_modify()));
    connect(ui->btnClose, SIGNAL(clicked()), this,SLOT(close()));
    connect(ui->btnDetail, SIGNAL(clicked()), this, SLOT(to_detail()));
    connect(ui->btnMeasure, SIGNAL(clicked()), this, SLOT(to_measure()));

    //数据库
    data_helper = new dataHelper();


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
    //ui->lcdNumber->setStyleSheet("background-image: url(:imaBackground/background.jpg)");
    ui->lcdNumber->display(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));

    time_slot();


    this->setAutoFillBackground(true);
    QLabel *my_label = new QLabel(this);
    my_label->setStyleSheet("background-color:rgb(50 , 204 , 194);");
    my_label->setGeometry(780, 0, this->width() - 780, this->height());
    my_label->lower();
    this->setStyleSheet("background-color:rgb(255 , 255 , 255);");


   QGraphicsDropShadowEffect *shadow_effect_1 = new QGraphicsDropShadowEffect(this);
   shadow_effect_1->setOffset(5, 5);
   shadow_effect_1->setColor(Qt::gray);
   shadow_effect_1->setBlurRadius(8);

   ui->customPlot->setGraphicsEffect(shadow_effect_1);
}

Member_Center::~Member_Center()
{
    delete data_helper;
    //qDeleteAll(this->weight_info); //如果QList <> 里面是指针类型,则需要这行代码来释放内存
    weight_info.clear();
    delete ui;
}

//获得用户信息
void Member_Center::setUserInfo(QString Id, QString Pwd)
{
    QString info[3];
    this->member_info.setId(Id);
    this->member_info.setPwd(Pwd);
    data_helper->Select("userInfo", Id, info);

    this->member_info.setName(info[0]);
    this->member_info.setSex(info[1]);
    this->member_info.setHeight(info[2]);


}

//获得理想体重
QString Member_Center::_getPerfectWeight()
{
    int weight;
    int height;
    height = member_info.getHeight().toInt();
    if (member_info.getSex() == "男") {
        weight = (height - 80) * 0.7;
    } else {
        weight = (height - 70) * 0.6;
    }
    return QString::number(weight);
}

//设置界面信息
void Member_Center::_setInterfaceInfo()
{
    ui->lblInfo->setText("欢迎<" + member_info.getName() + ">用户");
    ui->lblHeight->setText("身高" + member_info.getHeight() + "cm");
    ui->lblWeight->setText("理想体重为" + _getPerfectWeight() + "kg");
}

/*******************slot*******************/
//改变折线图的y轴范围
void Member_Center::to_change()
{
    if (!all_range) {
        ui->customPlot->yAxis->setRange(1, 100);
        all_range = true;
    } else {
        ui->customPlot->yAxis->rescale(true);
        all_range = false;
    }
    ui->customPlot->replot();
}

//修改用户信息(槽函数)
void Member_Center::to_modify()
{
    m_u_i = new modify_user_info();
    connect(m_u_i, SIGNAL(sucess(Member_Info)), this,SLOT(to_reshow(Member_Info)));
    m_u_i->setUserInfo(member_info);
    m_u_i->setInterfaceInfo();
    m_u_i->show();
}

//重绘窗体
void Member_Center::to_reshow(Member_Info mem_info)
{
    if (member_info != mem_info) {
        member_info = mem_info;
        _setInterfaceInfo();
    }
}

void Member_Center::to_detail()
{
    detail_weight = new Detail_Weight();
    detail_weight->getDetailWeight(&weight_info);
    detail_weight->initInterface();
    detail_weight->show();
}

void Member_Center::time_slot()
{
    ui->lcdNumber->setDigitCount(19);
    ui->lcdNumber->setSegmentStyle(QLCDNumber::Flat);
    ui->lcdNumber->display((new QDateTime) ->currentDateTime()
                           .toString("yyyy-MM-dd hh:mm:ss"));
}

void Member_Center::to_measure()
{
    double height;
    height =  member_info.getHeight().toDouble()   / 100 ;
    measuring_weight = new Measuring_Weight();
    connect(measuring_weight, SIGNAL(measure_to_close()), this, SLOT(_repaintGraph()));
    measuring_weight->getUserInfo(member_info.getId(), height);
    measuring_weight->showFullScreen();
    measuring_weight->show();
}

/*****************************************/

//窗体关闭事件
void Member_Center::closeEvent(QCloseEvent *event)
{
    emit to_close();
    QWidget::closeEvent(event);
}

//设置体重信息
void Member_Center::_setWeightInfo()
{
    weight_info.clear();
    QString sql_select = QString("select * from WeightInfo where Id = '%1' order by time ")
            .arg(member_info.getId());
    if (! data_helper->Select(sql_select, &weight_info) ) {
        QMessageBox message_box;
        message_box.setText("体重信息出错!");
        message_box.exec();
    }
}

//设置体重折线图
void Member_Center::_initWeightGraph()
{
    ui->customPlot->clearGraphs();
    ui->customPlot->xAxis->setLabel("测量次数(次)");
    ui->customPlot->yAxis->setLabel("体重(kg)");

    QVector <double> x(weight_info.size()), y(weight_info.size());

    for (int i = 0; i < weight_info.size(); i++) {
        x[i] = weight_info.at(i).getWeight().toDouble();
        y[i] = i + 1;
    }

    ui->customPlot->addGraph();
    ui->customPlot->graph(0)->setData(y, x);
    ui->customPlot->graph(0)->setPen(QPen(Qt::blue, 2));
    ui->customPlot->graph(0)->setName("变化趋势");
    ui->customPlot->rescaleAxes();

    ui->customPlot->addGraph(); // 红色点
    ui->customPlot->graph(1)->setPen(QPen(Qt::red, 3));
    ui->customPlot->graph(1)->setLineStyle(QCPGraph::lsNone);
    ui->customPlot->graph(1)->setScatterStyle(QCPScatterStyle::ssDisc);
    ui->customPlot->graph(1)->addData(y, x);
    ui->customPlot->graph(1)->setName("体重");

    ui->customPlot->xAxis->setRange(1, 30);
    ui->customPlot->xAxis->setAutoTickStep(false);  //不自动分配刻度间距
    ui->customPlot->xAxis->setTickStep(1.0); //设置步进为1
    ui->customPlot->xAxis->setSubTickLength(0); //不显示子刻度

    ui->customPlot->legend->setVisible(true);
    ui->customPlot->replot();
}

//初始化界面
void Member_Center::initInterface()
{
    this->_setInterfaceInfo();
    _setWeightInfo();
    _initWeightGraph();
}

void Member_Center::_repaintGraph()
{
    _setWeightInfo();
    _initWeightGraph();
}

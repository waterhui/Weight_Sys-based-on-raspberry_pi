#include "tourtest.h"
#include "ui_tourtest.h"

#include <QDebug>
#include <QThread>

tourTest::tourTest(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::tourTest)
{
    ui->setupUi(this);

    timer = new QTimer(this);

    connect(timer,SIGNAL(timeout()),this,SLOT(read_weight()));

    connect(ui->btnClose, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->btnReMeasure, SIGNAL(clicked()), this, SLOT(to_measure()));

    timer->setInterval(500);

    QLabel *my_label = new QLabel(this);
    my_label->setStyleSheet("background-color:rgb(50 , 204 , 194);");
    my_label->setGeometry(780, 0, this->width() - 780, this->height());
    my_label->lower();
    this->setStyleSheet("background-color:rgb(255 , 255 , 255);");



}


/*	测重过程
 *
 * 0. 隐藏groupBox, txtBser,再次测试按钮
 * 1. 测重(显示提示图标,提示语)
 * 2. 判断是否测重完成
 * 	 2.1 定义一个 double数组,size为3
 *  2.2 取最近三次的体重测试值,如果最近三次体重值相差不了1kg,则测重成功.
 * 3. 算出最终体重值
 *  2.3 算出三次的体重值,然后设置停顿0.5秒,最后提示用户测试完成,并且获得当前系统的时间,并记录下来
 * 4. 算出BMI指数,然后算出体重特征
 * 5. 显示出体重, BMI指数, 体重特征, 测试的当地时间, 相应评语
 * 6. 展示再次测试按钮.并将提示图标,提示语隐藏.
 *
 */


tourTest::~tourTest()
{
    delete ui;
}

/**********static***********/
int tourTest::real_weight_size = 3;

/*****************************slots****************************/
void tourTest::read_weight()
{
    //ui->lblWeight->setText(QString("%1").arg(hx->get_real_Weight(),0,'f',3));
    //ui->lblWeight->setText(ui->lblWeight->text().append("g"));

    //从hx711 sensor获得体重值
    double tmp_weight = (double)((int)(hx711::get_real_Weight() / 100)) / 10;
    //ui->lblWeight->setText(QString::number(tmp_weight, 'f', 1));
    if (tmp_weight <= 0) {
        ui->lblWeight->setText("0.0");
        return;
    }
    ui->lblWeight->setText(QString::number(tmp_weight, 'f', 1));
    real_weight[cur_array_pos] = tmp_weight;
#if 1
    ++cur_array_pos;
    if (cur_array_pos >= real_weight_size) {
        cur_array_pos = 0;
        is_can_check = true;
    }
    //如果测量测试达到最大范围real_weight_size,即可检测是否测试成功,
    if (is_can_check) {
        if (_isSuccess()) {
            timer->stop();
            _successToDo();
        }
    }
#endif
}

void tourTest::to_measure()
{
    _hideSomeObj();
    _showTips();
    timer->start();
}

/*************************************************************/

void tourTest::_hideSomeObj()
{
   //first(隐藏)
    ui->lblWeight->setText("0.0");
   ui->lblDetail->hide();					//隐藏详细体重信息
   ui->btnReMeasure->hide();			//再次测量体重按钮隐藏
}

void tourTest::_showSomeObj()
{
    ui->lblDetail->show();
    ui->btnReMeasure->show();
}

void tourTest::_showTips()
{
    //second(显示提示语)
    ui->lblTips->setText("请站到体重秤上");
    ui->lblTipsIcon->show();
}

void tourTest::_hideTips()
{
    ui->lblTipsIcon->hide();
}

//判断是否测量成功
bool tourTest::_isSuccess()
{
    bool flag = true;

    for (int i = 0; i < real_weight_size - 1; i++) {
        if ((int)real_weight[i] != (int)real_weight[i + 1]) {
            flag = false;
            break;
        }
    }
    return flag;

}

//获得测量成功的体重
double tourTest::_getSuccessWeight()
{
    double sum = 0.0;
    for (int i = 0; i < real_weight_size; i++) {
        sum += real_weight[i];
    }

    success_weight = ( (double)( (int)( ( sum / real_weight_size ) * 10 ) ) ) / 10;
    return success_weight;
}

QString tourTest::_getSuccessTime()
{
    return QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
}

void tourTest::to_twinkle()
{
    if (!is_twinkle) {
        ui->lblWeight->setText("");
        is_twinkle = true;
    }
    else {
        //ui->lblWeight->setText(QString::number(_getSuccessWeight(), 'f', 1));
        ui->lblWeight->setText(QString::number(success_weight, 'f', 1));
        is_twinkle = false;
    }
}

void tourTest::_successToDo()
{
        success_weight = _getSuccessWeight();
        ui->lblWeight->setText(QString::number(success_weight, 'f', 1));
        QTime t;
        t.start();
        QTimer *tmp_timer = new QTimer();
        tmp_timer->start(500);
        connect(tmp_timer, SIGNAL(timeout()), this, SLOT(to_twinkle()));
        while(t.elapsed() < 3000)
        {
            QCoreApplication::processEvents();
            QThread::usleep(10000);//sleep和usleep都已经obsolete，建议使用nanosleep代替
        }

        tmp_timer->stop();
        tmp_timer->destroyed();

        is_twinkle = true;
        delete tmp_timer;
        ui->lblWeight->setText(QString::number(success_weight, 'f', 1));
        cur_array_pos = 0;
        is_can_check = false;
        _hideTips();
        ui->lblDetail->setText("时间<" + _getSuccessTime() + ">");
        ui->lblTips->setText("测重成功！");
        _showSomeObj();
}

/***************************event***************************/

void tourTest::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    to_measure();
}

/**********************************************************/

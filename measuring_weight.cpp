#include "measuring_weight.h"
#include "ui_measuring_weight.h"

#include <QGraphicsDropShadowEffect>
#include <QThread>
#include <QDebug>

Measuring_Weight::Measuring_Weight(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Measuring_Weight)
{
    ui->setupUi(this);

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(read_weight()));
    timer->setInterval(500);

    /***slots***/
    connect(ui->btnClose, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->btnMeasure, SIGNAL(clicked()), this, SLOT(to_measure()));
    connect(ui->btnSave, SIGNAL(clicked()), this, SLOT(to_save()));
    /*********/

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

   ui->groupBoxComment->setGraphicsEffect(shadow_effect_1);

   data_helper = new dataHelper();

   this->setAttribute(Qt::WA_DeleteOnClose);

}

Measuring_Weight::~Measuring_Weight()
{
    delete data_helper;
    delete ui;
}

/********static********/
int Measuring_Weight::real_weight_size = 3;

/********************/


/***********************slots*********************/

void Measuring_Weight::to_measure()
{
    ui->lblWeight->setText("0.0");
    this->_hideSomeObj();
    this->_showTips();
    timer->start();
}

void Measuring_Weight::to_save()
{
    //判断体重记录是否大于30条
    if( _isSatisfyMaxLimit()) {
        //删除一条,然后在保存最新那条
        _removeFirstWeight();
    }
    QString sql_save = QString("insert into WeightInfo values('%1', '%2', '%3', '%4', '%5')")
            .arg(this->user_id)
            .arg(QString::number(this->success_weight, 'f', 1))
            .arg(QString::number(this->success_bmi, 'f', 1))
            .arg(this->success_status)
            .arg(this->success_time);

    data_helper->Insert(sql_save);

    ui->btnSave->hide();
    ui->lblTips->show();
    ui->lblTips->setText("体重保存成功！");
}

void Measuring_Weight::read_weight()
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

}

void Measuring_Weight::to_twinkle()
{
    if (!is_twinkle) {
        ui->lblWeight->setText("");
        is_twinkle = true;
    }
    else {
        ui->lblWeight->setText(QString::number(_getSuccessWeight(), 'f', 1));
        is_twinkle = false;
    }
}

/************************************************/



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

void Measuring_Weight::_hideSomeObj()
{
    ui->groupBoxComment->hide();
    ui->txtBserCom->hide();
    ui->btnMeasure->hide();
    ui->btnSave->hide();
    ui->lblDetail->hide();
}

void Measuring_Weight::_showSomeObj()
{
    ui->groupBoxComment->show();
    ui->txtBserCom->show();
    ui->btnMeasure->show();
    ui->btnSave->show();
    ui->lblDetail->show();
}


void Measuring_Weight::_hideTips()
{
    ui->lblTips->hide();
    ui->lblTipsIcon->hide();
}

void Measuring_Weight::_showTips()
{
    ui->lblTips->setText("请站到体重秤上");
    ui->lblTips->show();
    ui->lblTipsIcon->show();
}


bool Measuring_Weight::_isSuccess()
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

double Measuring_Weight::_getSuccessWeight()
{
    double sum = 0.0;
    for (int i = 0; i < real_weight_size; i++) {
        sum += real_weight[i];
    }

    success_weight = ( (double)( (int)( ( sum / real_weight_size ) * 10 ) ) ) / 10;
    return success_weight;
}

QString Measuring_Weight::_getSuccessTime()
{
    return QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
}

double Measuring_Weight::_getBMI()
{
   return ( (double)( (int)( ( success_weight / (user_height * user_height) ) * 10 ) ) ) / 10;
}

QString Measuring_Weight::_getStatus()
{
    /*中国标准
     *
     *	过轻 					< 18.4
     * 健康 					18.5 - 23.9
     *	过重 					24.0 - 27.9
     * 肥胖 					28 - 31.9
     * 极度肥胖 			高于32
     */
    QString tmp_status;
    if (success_bmi < 18.4)
        tmp_status = "过轻";
    else if (success_bmi >= 18.5 && success_bmi <= 23.9)
        tmp_status = "健康";
    else if (success_bmi >= 24.0 && success_bmi <= 27.9)
        tmp_status = "过重";
    else if (success_bmi >= 28 && success_bmi <= 31.9)
        tmp_status = "肥胖";
    else
        tmp_status = "极度肥胖";

    return tmp_status;
}

void Measuring_Weight::_successToDo()
{
    success_weight = _getSuccessWeight();
    ui->lblWeight->setText(QString::number(_getSuccessWeight(), 'f', 1));
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
    ui->lblWeight->setText(QString::number(_getSuccessWeight(), 'f', 1));
    tmp_timer->destroyed();

    is_twinkle = true;
    delete tmp_timer;
    //获得体重值

    //获得Time
    success_time  = _getSuccessTime();

    //获得BMI
    success_bmi = _getBMI();

    //获得Status
    success_status = _getStatus();

    //显示SomeObj
    _showSomeObj();
    //隐藏Tips
    _hideTips();

    //获得Remark
    ui->txtBserCom->setText(_getRemark());

    ui->lblDetail->setText("测试结果:\nBMI值<" + QString::number(success_bmi, 'f', 1)+ ">, " + "体态<" + success_status + ">, " + "时间<" + success_time + ">");

}

bool Measuring_Weight::_isSatisfyMaxLimit()
{
    QString sql_select = QString("select * from WeightInfo where Id = '%1' order by Time")
            .arg(this->user_id);
    if(data_helper->SelectWeight(sql_select,first_time) != -1) {
        return true;
    }
    return false;
}

bool Measuring_Weight::_removeFirstWeight()
{
    QString sql_delete = QString("delete from WeightInfo where Id = '%1' and Time = '%2' ")
            .arg(user_id)
            .arg(first_time);

    return data_helper->Delete(sql_delete);
}

/********************public******************/

void Measuring_Weight::getUserInfo(QString id, double height)
{
    this->user_id  = id;
    this->user_height = height;
}


void Measuring_Weight::_initInterface()
{
    this->_hideSomeObj();
    this->_showTips();
}

QString Measuring_Weight::_getRemark()
{
    QString tmp_remark;
    QString sql_remark = QString("select Remark from Comment where Status = '%1' ")
            .arg(success_status);
    data_helper->Select(sql_remark, tmp_remark);
    return tmp_remark;
}

/******************************************/


/************event************/

void Measuring_Weight::showEvent(QShowEvent *event)
{
    _initInterface();
    QWidget::showEvent(event);
    to_measure();
}

void Measuring_Weight::closeEvent(QCloseEvent *event)
{
    emit measure_to_close();
    QWidget::closeEvent(event);
}

/****************************/

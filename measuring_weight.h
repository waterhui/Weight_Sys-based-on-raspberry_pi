#ifndef MESURING_WEIGHT_H
#define MESURING_WEIGHT_H

#include <QWidget>
#include <QDateTime>
#include <QShowEvent>
#include <QCloseEvent>
#include <QTimer>
#include "datahelper.h"
#include "hx711.h"

namespace Ui {
class Measuring_Weight;
}

class Measuring_Weight : public QWidget
{
    Q_OBJECT

public:
    explicit Measuring_Weight(QWidget *parent = 0);
    ~Measuring_Weight();

    void getUserInfo(QString id, double height);
    void _initInterface();

    void closeEvent(QCloseEvent*);

protected slots:
    void to_measure();
    void to_save();
    void read_weight();
    void to_twinkle();

signals:
    void measure_to_close();

private:
   void  _hideSomeObj();					//隐藏某些控件
   void _showSomeObj();				//显示某些控件
   void _hideTips();							//隐藏提示
   void _showTips();						//显示提示
   bool _isSuccess(); 						//判断是否读取体重成功
   double _getSuccessWeight();		//获得测重成功的体重值
   QString _getSuccessTime();			//获得测重成功的时间
   double _getBMI();						//获得该用户该次测试的BMI值
   QString _getStatus();					//获得该用户该次测试的体态
   QString _getRemark();					//获得该用户该次测试的评语
   void _successToDo();					//测重成功后需要做的一些操作
   bool _isSatisfyMaxLimit();				//判断是否大于用户体重值是否大于30次,
   bool _removeFirstWeight();			//如果用户体重大于30次,则消除时间最遥远的那一次

    void showEvent(QShowEvent*);

private:
    Ui::Measuring_Weight *ui;
    dataHelper *data_helper;

    QTimer *timer;
    static int real_weight_size;
    double real_weight[3];
    QString user_id;							//用户id
    double user_height;					//用户身高(m)
    double success_weight;				//用户成功测试体重值(kg)
    double success_bmi;					//用户该次体重测试的BMI值
    QString success_status;				//用户该次体重测试的体态
    QString success_time;					//用户该次提冲测试的完成时间
    QString first_time;						//用户在数据库中测试体重时间的最早时间
    int cur_array_pos = 0;				//当前体重数组的位置
    bool is_can_check = false;			//判断是否能够进行判断体重
    bool is_twinkle = true;				//闪烁标志位

};

#endif // MESURING_WEIGHT_H

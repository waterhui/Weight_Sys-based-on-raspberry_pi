#ifndef TOURTEST_H
#define TOURTEST_H

#include <QWidget>
#include "hx711.h"
#include <QTimer>
#include <QDateTime>
#include <QShowEvent>

#include "datahelper.h"

namespace Ui {
class tourTest;
}

class tourTest : public QWidget
{
    Q_OBJECT

public:
    explicit tourTest(QWidget *parent = 0);
    ~tourTest();

    void showEvent(QShowEvent *);

public slots:
            void read_weight();
            void to_measure();
            void to_twinkle();

private:
            void _hideSomeObj(); 	//隐藏某些控件
            void _showSomeObj(); 	//显示某些控件
            void _showTips();			//显示提示控件
            void _hideTips();			//隐藏提示控件
            bool _isSuccess(); 			//判断是否读取体重成功
            double _getSuccessWeight();	//获得测重成功的体重值
            QString _getSuccessTime();
            void _successToDo();	//测重成功后需要做的一些操作

private:
    Ui::tourTest *ui;
    QTimer *timer;
    static int real_weight_size;
    double real_weight[3];
    double success_weight;
    QString success_time;
    int cur_array_pos = 0;			//当前体重数组的位置
    bool is_can_check = false;		//判断是否能够进行判断体重
    bool is_twinkle = true;

};

#endif // TOURTEST_H

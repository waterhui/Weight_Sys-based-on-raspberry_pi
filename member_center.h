#ifndef MEMBER_CENTER_H
#define MEMBER_CENTER_H

#include <QWidget>
#include <QLinkedList>
#include <QList>
#include <QDebug>

#include "datahelper.h"
#include "member_info.h"
#include "modify_user_info.h"
#include "weight_info.h"
#include "detail_weight.h"
#include "measuring_weight.h"

namespace Ui {
class Member_Center;
}

class Member_Center : public QWidget
{
    Q_OBJECT

public:
    explicit Member_Center(QWidget *parent = 0);
    ~Member_Center();

    void setUserInfo(QString, QString); //设置member_info对象
    void initInterface(); //初始化界面
    void closeEvent(QCloseEvent *);

protected slots:
    void to_change(); //改变折线图刻度
    void to_modify(); //修改用户信息
    void to_reshow(Member_Info); //重绘界面
    void to_detail(); //展示用户体重详细信息
    void to_measure(); 	//测量体重
    void time_slot();
    void _repaintGraph();		//重画折线图

 signals:
    void to_close();

private:
    void _setInterfaceInfo(); //设置界面信息(QLabel)
    QString _getPerfectWeight(); //获得当前用户的最佳体重
    void _setWeightInfo(); //设置用户的历史体重信息
    void _initWeightGraph();	//设置折线图

private:
    Ui::Member_Center *ui;
    bool all_range;
    Member_Info member_info;
    dataHelper *data_helper;
    modify_user_info * m_u_i;
    Detail_Weight *detail_weight;
    Measuring_Weight *measuring_weight;

    QList <Weight_Info> weight_info;
};

#endif // MEMBER_CENTER_H

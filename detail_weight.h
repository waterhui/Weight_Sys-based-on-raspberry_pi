#ifndef DETAIL_WEIGHT_H
#define DETAIL_WEIGHT_H

#include <QWidget>
#include <weight_info.h>
#include <QList>
#include <QMouseEvent>
#include <QPaintEvent>

#include "datahelper.h"
#include "message_box.h"

namespace Ui {
class Detail_Weight;
}

class Detail_Weight : public QWidget
{
    Q_OBJECT

public:
    explicit Detail_Weight(QWidget *parent = 0);
    ~Detail_Weight();

    void getDetailWeight(QList <Weight_Info> *);
    void initInterface();
    void setAdminMode();		//设置成管理员模式
    void setUserWeight(QString);

    virtual void  mouseMoveEvent(QMouseEvent *);
    virtual void mousePressEvent(QMouseEvent *);
    virtual void mouseReleaseEvent(QMouseEvent *);
    void paintEvent(QPaintEvent *event);

protected slots:
    void to_Exit();
    void to_delete();
    void to_delete_all();

private:
    void _setTableViewData();
    void _clearTableViewData();
    bool _isSelectAnyWeight();

private:
    Ui::Detail_Weight *ui;
    QList <Weight_Info> *weight_info;
    dataHelper *data_helper;
    bool isAdminMode = false;
    bool isMousePressed = false; //判断鼠标是否按下
    QPoint mousePoint; //鼠标相对于窗体的位置
};

#endif // DETAIL_WEIGHT_H

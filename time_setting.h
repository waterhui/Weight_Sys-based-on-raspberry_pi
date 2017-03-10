#ifndef TIME_SETTING_H
#define TIME_SETTING_H

#include <QWidget>
#include <QDateTime>

#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainterPath>
#include <QPainter>
#include <QtMath>
#include <QLabel>

#include "message_box.h"

namespace Ui {
class Time_Setting;
}

class Time_Setting : public QWidget
{
    Q_OBJECT

public:
    explicit Time_Setting(QWidget *parent = 0);
    ~Time_Setting();

    virtual void  mouseMoveEvent(QMouseEvent *);
    virtual void mousePressEvent(QMouseEvent *);
    virtual void mouseReleaseEvent(QMouseEvent *);
    void paintEvent(QPaintEvent *event);

protected slots:
    void to_ok();

private:
    Ui::Time_Setting *ui;

    bool isMousePressed; //判断鼠标是否按下
    QPoint mousePoint; //鼠标相对于窗体的位置
};

#endif // TIME_SETTING_H

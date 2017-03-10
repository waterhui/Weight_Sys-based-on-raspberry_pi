#ifndef ADMIN_MODIFY_PWD_H
#define ADMIN_MODIFY_PWD_H

#include <QWidget>
#include <QLineEdit>

#include "keyboard.h"
#include "member_info.h"
#include "datahelper.h"
#include "message_box.h"


#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainterPath>
#include <QPainter>
#include <QtMath>

namespace Ui {
class Admin_Modify_Pwd;
}

class Admin_Modify_Pwd : public QWidget
{
    Q_OBJECT

public:
    explicit Admin_Modify_Pwd(QWidget *parent = 0);
    ~Admin_Modify_Pwd();

    void setAdminInfo(Member_Info);

    bool eventFilter(QObject *watched, QEvent *event);

    virtual void  mouseMoveEvent(QMouseEvent *);
    virtual void mousePressEvent(QMouseEvent *);
    virtual void mouseReleaseEvent(QMouseEvent *);
    void paintEvent(QPaintEvent *event);

protected slots:
    void to_ok();
    void to_edit(QString);
    void to_reshow();

signals:
    void success(Member_Info);

private:
    bool _isFillAll();
    bool _isConsistent();

private:
    Ui::Admin_Modify_Pwd *ui;
    Member_Info admin_info;
    dataHelper *data_helper;
    keyBoard *key;
    QLineEdit *select_edit;

    bool isMousePressed; //判断鼠标是否按下
    QPoint mousePoint; //鼠标相对于窗体的位置
};

#endif // ADMIN_MODIFY_PWD_H

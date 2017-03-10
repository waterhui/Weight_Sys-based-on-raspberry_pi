#ifndef MODIFY_USER_INFO_H
#define MODIFY_USER_INFO_H

#include <QWidget>
#include <QLineEdit>
#include <QString>

#include "keyboard.h"
#include "member_info.h"
#include "datahelper.h"
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainterPath>
#include <QPainter>
#include <QtMath>
#include "message_box.h"

namespace Ui {
class modify_user_info;
}

class modify_user_info : public QWidget
{
    Q_OBJECT

public:
    explicit modify_user_info(QWidget *parent = 0);
    ~modify_user_info();

    void setUserInfo(Member_Info);
    void setInterfaceInfo();

    virtual void  mouseMoveEvent(QMouseEvent *);
    virtual void mousePressEvent(QMouseEvent *);
    virtual void mouseReleaseEvent(QMouseEvent *);
    void paintEvent(QPaintEvent *event);

signals:
    void sucess(Member_Info);

public slots:
    void to_cancel();
    void to_hide();
    void to_ok();
    void to_edit(QString);

  protected:
    bool eventFilter(QObject *, QEvent *);

private:
    void _setUserInfo();
    bool _isFillAll();
    bool _isMatchPwd();

private:
    Ui::modify_user_info *ui;
    keyBoard *key = NULL;
    QLineEdit *select_LE;
    Member_Info mem_info;
    dataHelper *data_helper = NULL;
    bool isMousePressed; //判断鼠标是否按下
    QPoint mousePoint; //鼠标相对于窗体的位置

};

#endif // MODIFY_USER_INFO_H

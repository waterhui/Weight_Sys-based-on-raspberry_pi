#ifndef MODIFY_COMMENT_H
#define MODIFY_COMMENT_H

#include <QWidget>

#include "keyboard.h"
#include "comment_info.h"
#include "datahelper.h"
#include "message_box.h"


#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainterPath>
#include <QPainter>
#include <QtMath>

namespace Ui {
class Modify_Comment;
}

class Modify_Comment : public QWidget
{
    Q_OBJECT

public:
    explicit Modify_Comment(QWidget *parent = 0);
    ~Modify_Comment();

    bool eventFilter(QObject *watched, QEvent *event);

    virtual void  mouseMoveEvent(QMouseEvent *);
    virtual void mousePressEvent(QMouseEvent *);
    virtual void mouseReleaseEvent(QMouseEvent *);
    void paintEvent(QPaintEvent *event);

    void setCommentInfo(Comment_Info);
    void initInterface();

protected slots:
    void to_hide();
    void to_edit(QString);
    void to_ok();

signals:
    void to_success(Comment_Info);

private:
    Ui::Modify_Comment *ui;
    Comment_Info comment_info;
    keyBoard *key;
    dataHelper *data_helper;

    bool isMousePressed; //判断鼠标是否按下
    QPoint mousePoint; //鼠标相对于窗体的位置
};

#endif // MODIFY_COMMENT_H

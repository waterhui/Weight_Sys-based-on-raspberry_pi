#ifndef MANAGE_COMMENT_INFO_H
#define MANAGE_COMMENT_INFO_H

#include <QWidget>

#include "datahelper.h"
#include "comment_info.h"
#include "modify_comment.h"
#include "message_box.h"
#include <QStandardItemModel>
#include <QStandardItem>

#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainterPath>
#include <QPainter>
#include <QtMath>

namespace Ui {
class Manage_Comment_Info;
}

class Manage_Comment_Info : public QWidget
{
    Q_OBJECT

public:
    explicit Manage_Comment_Info(QWidget *parent = 0);
    ~Manage_Comment_Info();

    void initInterface();

    virtual void  mouseMoveEvent(QMouseEvent *);
    virtual void mousePressEvent(QMouseEvent *);
    virtual void mouseReleaseEvent(QMouseEvent *);
    void paintEvent(QPaintEvent *event);

protected slots:
    void to_show_detail(QModelIndex index);
    void to_modify();
    void to_reshow(Comment_Info);

private:
    void _setCommentInfo();

private:
    Ui::Manage_Comment_Info *ui;
    QList <Comment_Info> comment_info;
    dataHelper *data_helper;
    Modify_Comment *modify_comment;
    bool isMousePressed; //判断鼠标是否按下
    QPoint mousePoint; //鼠标相对于窗体的位置
};

#endif // MANAGE_COMMENT_INFO_H

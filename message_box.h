#ifndef MESSAGE_BOX_H
#define MESSAGE_BOX_H

#include <QWidget>
#include <QDialog>


#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainterPath>
#include <QPainter>
#include <QtMath>
#include <QLabel>

namespace Ui {
class Message_Box;
}

class Message_Box : public QDialog
{
    Q_OBJECT

public:
    explicit Message_Box(QDialog *parent = 0);
    ~Message_Box();

    virtual void  mouseMoveEvent(QMouseEvent *);
    virtual void mousePressEvent(QMouseEvent *);
    virtual void mouseReleaseEvent(QMouseEvent *);
    void paintEvent(QPaintEvent *event);

    void setText(QString);
    void setTitle(QString);
    void setImg(int);
    void setImg(QString);
    void setShowButton(bool, bool);

    static int Warning;
    static int Question;

private:
    Ui::Message_Box *ui;

    bool isMousePressed; //判断鼠标是否按下
    QPoint mousePoint; //鼠标相对于窗体的位置
    static QString str_warning;
    static QString str_question;
};

#endif // MESSAGE_BOX_H

#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <QWidget>
#include <QPushButton>
#include <QAbstractButton>
#include <QFile>
#include <QString>
#include <QStringList>
#include <QFocusEvent>

#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainterPath>
#include <QPainter>
#include <QtMath>

namespace Ui {
class  keyBoard;
}

class keyBoard : public QWidget
{
    Q_OBJECT

public:
    explicit keyBoard(QWidget *parent = 0);
    ~keyBoard();
    void display() ;
    void setLeText(QString);
    void SetPwdMode();
    void SetNormalMode();

    bool eventFilter(QObject *, QEvent *);
    //virtual void  mouseMoveEvent(QMouseEvent *);
    //virtual void mousePressEvent(QMouseEvent *);
    //virtual void mouseReleaseEvent(QMouseEvent *);

signals:
        void Hide();
        void Edit(QString);
public slots:
    void enter(QAbstractButton*) ;
    void GetText(QAbstractButton*) ;
    void printHZ(QAbstractButton*) ;
    void printNum(QAbstractButton*);
    void toEdit();

private:
    bool _deleteSelect();

private:
    Ui::keyBoard *ui;
    bool Cap ;
    bool CH_EN ;
    //QFile *file ;
    QString str ;
    QStringList HZK ;
    QString HZstr ;
    int curPageNum ;
    QString Pystr ;

    int curLineEditPos = 0; //当前文本框的位置
    int select_start_pos;
    int select_len;
    bool is_select;


    bool isMousePressed; //判断鼠标是否按下
    QPoint mousePoint; //鼠标相对于窗体的位置
};

#endif // WIDGET_H

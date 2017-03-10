#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTimer>
#include <QString>
#include <QMessageBox>
#include "keyboard.h"
#include "tourtest.h"
#include "register.h"
#include "member_center.h"
#include "datahelper.h"
#include "admin_center.h"
#include "message_box.h"
#include "hx711.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

    bool eventFilter(QObject *obj, QEvent *event);

public slots:
            void to_measure();
            void reFocus();
            void Edit(QString);
            void toReg();
            void toLogin();
            void time_slot();

private:
    Ui::Widget *ui;
    QTimer *timer_Edit;
    keyBoard *key;
    QLineEdit  *leUsr;
    QLineEdit  *lePwd;
    QLineEdit  *le_select;
    Register *reg;
    Member_Center *mem_ctr;
    Admin_Center *admin_ctr;
    tourTest *tourT;

    dataHelper *data_helper;
};

#endif // WIDGET_H

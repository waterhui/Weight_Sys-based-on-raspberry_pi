#ifndef REGISTER_H
#define REGISTER_H

#include <QWidget>
#include <QMessageBox>
#include <QLineEdit>

#include "datahelper.h"
#include "keyboard.h"
#include "member_info.h"
#include "message_box.h"

namespace Ui {
class Register;
}

class Register : public QWidget
{
    Q_OBJECT

public:
    explicit Register(QWidget *parent = 0);
    ~Register();

public slots:
    void toOk();
    void toCancel();
    void Edit(QString);
    void Hide();
    void time_slot();

protected:
    bool eventFilter(QObject *, QEvent *);

private:
    bool isFillAll();
    Ui::Register *ui;
    dataHelper *data_helper;
    keyBoard *key;
    QLineEdit *select_LE;
};

#endif // REGISTER_H

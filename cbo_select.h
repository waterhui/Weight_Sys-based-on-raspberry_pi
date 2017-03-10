#ifndef CBO_SELECT_H
#define CBO_SELECT_H

#include <QWidget>

namespace Ui {
class cbo_select;
}

class cbo_select : public QWidget
{
    Q_OBJECT

public:
    explicit cbo_select(QWidget *parent = 0);
    ~cbo_select();

signals:
    void re_select(QString, int);

protected slots:
    void to_user();
    void to_name();
    void to_sex();
    void to_height();

private:
    Ui::cbo_select *ui;
};

#endif // CBO_SELECT_H

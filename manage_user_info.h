#ifndef MANAGE_USER_INFO_H
#define MANAGE_USER_INFO_H

#include <QWidget>

#include <QDebug>
#include <QString>
#include <QList>
#include <QMainWindow>
#include <QStandardItemModel>
#include <QEvent>

#include "datahelper.h"
#include "member_info.h"
#include "keyboard.h"
#include "modify_user_info.h"
#include "detail_weight.h"
#include "message_box.h"
#include "cbo_select.h"

namespace Ui {
class Manage_User_Info;
}

class Manage_User_Info : public QWidget
{
    Q_OBJECT

public:
    explicit Manage_User_Info(QWidget *parent = 0);
    ~Manage_User_Info();
#if 1
    void initInterface();

    bool eventFilter(QObject *, QEvent *);
    void mousePressEvent(QMouseEvent *event);
#endif

protected slots:

#if 1
   void to_query();
   void to_query_all();
   void to_delete();
   void to_delete_all();
   void to_edit(QString);
   void to_reshow();
   void to_modify_userInfo();
   void to_modify_weightInfo();
   void to_reset_tableview(Member_Info);
   void to_select();
   void to_re_select(QString, int);
#endif

private:
#if 1
    void _setTableViewData();
    void _clearTableViewData();
    bool _isSelectAnyUser();
    void _getAllUserInfo();
#endif

private:
    Ui::Manage_User_Info *ui;
    dataHelper *data_helper;
    QList <Member_Info> member_info;
    keyBoard *key;
    modify_user_info *m_u_i; //修改界面
    Detail_Weight *detail_weight; //修改体重信息界面
    cbo_select *my_cbo;
    int cur_index = 0;
};

#endif // MANAGE_USER_INFO_H

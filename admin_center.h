#ifndef ADMIN_CENTER_H
#define ADMIN_CENTER_H

#include <QWidget>
#include <QDateTime>
#include <QTimer>

#include "manage_user_info.h"
#include "manage_comment_info.h"
#include "admin_modify_pwd.h"
#include "time_setting.h"

namespace Ui {
class Admin_Center;
}

class Admin_Center : public QWidget
{
    Q_OBJECT

public:
    explicit Admin_Center(QWidget *parent = 0);
    ~Admin_Center();

    void closeEvent(QCloseEvent *event);
    void setAdminInfo(QString, QString);

protected slots:
    void time_slot();
    void to_userInfo();
    void to_commentInfo();
    void to_modify_pwd();
    void to_reshow(Member_Info);
    void to_setting_time();

signals:
    void to_close();

private:
    Ui::Admin_Center *ui;
    Manage_User_Info * manage_user_info;
    Manage_Comment_Info *manage_comment_info;
    Member_Info admin_info;
    Admin_Modify_Pwd *admin_modify_pwd;
    Time_Setting *time_setting;
};

#endif // ADMIN_CENTER_H

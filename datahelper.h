#ifndef DATAHELPER_H
#define DATAHELPER_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QString>
#include <QSqlQueryModel>
#include <QString>
#include <QMessageBox>
#include <QList>

#include "weight_info.h"
#include "member_info.h"
#include "comment_info.h"

class dataHelper
{
    //Q_OBJECT
public:
    dataHelper();
    ~dataHelper();
    bool SqlConnect();
    void SqlClose();

    bool Select(QString);  //查询
    bool Select(QString, QString,QString*); //获得单个用户信息
    bool Select(QString, QList<Member_Info> *);
    int   Select(QString, QList<Weight_Info> *); //获得单个用户体重
    bool Select(QString, QList<Comment_Info> *); //获得所有评语
    bool Select(QString, QString &);
    int SelectWeight(QString, QString &);

    bool Update(QString); //更新

    bool Delete(QString); //删除

    bool Insert(QString); //插入

private:
    QSqlDatabase db;
};

#endif // DATAHELPER_H

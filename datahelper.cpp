#include "datahelper.h"

#include <QDebug>


dataHelper::dataHelper()
{
    /*
    qDebug() << QSqlDatabase::contains("qt_sql_default_connection");
    if(QSqlDatabase::contains("qt_sql_default_connection"))
        db = QSqlDatabase::database("qt_sql_default_connection");
    else
        db = QSqlDatabase::addDatabase("QSQLITE");
    */
    //db = QSqlDatabase::addDatabase("QSQLITE");
    if (QSqlDatabase::contains("wth")) {
        db = QSqlDatabase::database("wth");
    } else {
        db = QSqlDatabase::addDatabase("QSQLITE", "wth");
        db.setDatabaseName("./database/weight.db");
    }
    //db.setDatabaseName("/home/wth/qtTest/register/database/weight.db");
}

dataHelper::~dataHelper()
{

}


bool dataHelper::SqlConnect()
{
    bool respone;
    respone = db.open();
    if (!respone) {
        QMessageBox message_box;
        message_box.setText("数据库出错");
    }
    return respone;
}

void dataHelper::SqlClose()
{
    db.close();
}

//查询是否有该用户
bool dataHelper::Select(QString sql)
{
    if (!SqlConnect()) {
        SqlClose();
        return false;
    }

    QSqlQuery query(db);
    query.exec(sql);
    bool respone = query.next();
    SqlClose();
    return respone;
}

//获得用户信息
bool dataHelper::Select(QString tableName, QString Id,QString *info)
{
    if (!SqlConnect()) {
        SqlClose();
        return false;
    }
    QString select = QString("select * from %1 where Id = '%2' ")
            .arg(tableName)
            .arg(Id);
    QSqlQuery query(db);
    query.exec(select);
    SqlClose();

    if(query.next()) {
        info[0] = query.value("Name").toString();
        info[1] = query.value("Sex").toString();
        info[2] = query.value("Height").toString();
    }
    return true;
}

//获得所有用户的信息
bool dataHelper::Select(QString sql, QList<Member_Info> * member_info)
{
    if (!SqlConnect()) {
        SqlClose();
        return false;
    }

    QSqlQuery query(db);
    bool respone  = query.exec(sql);

    if (!respone)
        return false;

    Member_Info member;
    QString select_pwd;
    QSqlQuery query_pwd(db);
    while (query.next()) {
        member.setId(query.value("Id").toString());
        member.setName(query.value("Name").toString());
        member.setSex(query.value("Sex").toString());
        member.setHeight(query.value("Height").toString());

        select_pwd = QString("select * from Login where Id = '%1' ").arg(query.value("Id").toString());
        query_pwd.exec(select_pwd);
        if (query_pwd.first())
            member.setPwd(query_pwd.value("Pwd").toString());
        else
            return false;

        member_info->append(member);
    }

    SqlClose();
    return true;
}

//查询所有该用户体重信息
int dataHelper::Select(QString sql, QList<Weight_Info> * weight_info)
{
    if (!SqlConnect()) {
        SqlClose();
        return 0;
    }

    QSqlQuery query(db);
    bool respone = query.exec(sql);

    if(!respone) {
        SqlClose();
        return -1;
    }

    Weight_Info weight;
    while (query.next()) {
        weight.setId(query.value("Id").toString());
        weight.setWeight(query.value("Weight").toString());
        weight.setBMI(query.value("BMI").toString());
        weight.setStatus(query.value("Status").toString());
        weight.setTime(query.value("Time").toString());

        weight_info->append(weight);
    }

    SqlClose();
    return 1;
}

bool dataHelper::Select(QString sql, QList<Comment_Info> *comment_info)
{
    if (!SqlConnect()) {
        SqlClose();
        return false;
    }

    QSqlQuery query(db);
    bool respone = query.exec(sql);
    if (!respone)
        return false;

    Comment_Info comment;
    while(query.next()) {
        comment.setStatus(query.value("Status").toString());
        comment.setRemark(query.value("Remark").toString());

        comment_info->append(comment);
    }

    SqlClose();

    return true;
}

bool dataHelper::Select(QString sql, QString &remark)
{
    if (!SqlConnect()) {
        SqlClose();
        return false;
    }


    QSqlQuery query(db);
    bool respone = query.exec(sql);
    if (!respone)
        return false;

    while(query.next()) {
        remark = query.value("Remark").toString();
    }

    SqlClose();
    return true;
}

int dataHelper::SelectWeight(QString sql, QString &first_time)
{
    if (!SqlConnect()) {
        SqlClose();
        return 0;
    }


    QSqlQuery query(db);
    bool respone = query.exec(sql);
    if (!respone)
        return 0;

    query.first();
    int num = 1;

   first_time = query.value("Time").toString();
    while(query.next()) {
        num++;
    }

    SqlClose();
    if (num >= 30) {
        return 1;
    }
    return -1;
}

bool dataHelper::Delete(QString sql)
{
    if (!SqlConnect()) {
        SqlClose();
        return 0;
    }

    QSqlQuery query(db);
    bool respone = query.exec(sql);
    SqlClose();
    return respone;
}

bool dataHelper::Insert(QString sql)
{
    if (!SqlConnect()) {
        SqlClose();
        return 0;
    }

    QSqlQuery query(db);
    bool respone = query.exec(sql);
    SqlClose();
    return respone;
}

//更新
bool dataHelper::Update(QString update)
{
    if (!SqlConnect()) {
        SqlClose();
        return false;
    }

    QSqlQuery query(db);
    bool respone = query.exec(update);
    SqlClose();

    return respone;
}


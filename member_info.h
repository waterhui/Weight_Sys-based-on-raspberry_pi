#ifndef MEMBER_INFO_H
#define MEMBER_INFO_H

#include <QString>

class Member_Info
{
public:
    Member_Info();
    bool operator !=(const Member_Info &) const;
    void setId(QString);
    void setPwd(QString);
    void setName(QString);
    void setSex(QString);
    void setHeight(QString);

    const QString& getId() const;  //在该成员函数内部,this的类型是一个指向const类型对象的const指针。
    const QString& getPwd() const;
    const QString& getName() const;
    const QString& getSex() const;
    const QString& getHeight() const;



private:
        QString Id;
        QString Pwd;
        QString Name;
        QString Sex;
        QString Height;
};

#endif // MEMBER_INFO_H

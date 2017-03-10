#ifndef COMMENT_INFO_H
#define COMMENT_INFO_H

#include <QString>

class Comment_Info
{
public:
    Comment_Info();

    void setStatus(QString);
    void setRemark(QString);

    const QString& getStatus() const;
    const QString& getRemark() const;

private:
    QString status;   //体重状态
    QString remark; //评语
};

#endif // COMMENT_INFO_H

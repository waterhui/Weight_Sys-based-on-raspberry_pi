#include "comment_info.h"

Comment_Info::Comment_Info()
{

}

void Comment_Info::setStatus(QString status)
{
    this->status = status;
}

void Comment_Info::setRemark(QString remark)
{
    this->remark = remark;
}

const QString& Comment_Info::getStatus() const
{
    return this->status;
}

const QString& Comment_Info::getRemark() const
{
    return this->remark;
}

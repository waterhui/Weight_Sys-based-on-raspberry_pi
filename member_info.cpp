#include "member_info.h"

Member_Info::Member_Info()
{

}

//set
void Member_Info::setId(QString id)
{
    this->Id = id;
}

void Member_Info::setPwd(QString pwd)
{
    this->Pwd = pwd;
}

void Member_Info::setName(QString name)
{
    this->Name = name;
}

void Member_Info::setSex(QString sex)
{
    this->Sex = sex;
}

void Member_Info::setHeight(QString height)
{
    this->Height = height;
}

//get
const QString& Member_Info::getId() const
{
    return this->Id;
}

const QString& Member_Info::getPwd() const
{
    return this->Pwd;
}

const QString& Member_Info::getName() const
{
    return this->Name;
}


const QString& Member_Info::getSex() const
{
    return this->Sex;
}

const QString& Member_Info::getHeight() const
{
    return this->Height;
}

bool Member_Info::operator !=(const Member_Info &mem_info) const
{

    bool flag = false;
    if(this->getName() != mem_info.getName()) //const对象不允许调用非const成员函数
        flag = true;
    else if(this->getPwd() != mem_info.getPwd())
        flag = true;
    else if(this->getSex() != mem_info.getSex())
        flag = true;
    else if(this->getHeight() != mem_info.getHeight())
        flag = true;

    return flag;
}

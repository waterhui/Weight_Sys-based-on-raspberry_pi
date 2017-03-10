#include "weight_info.h"

Weight_Info::Weight_Info()
{

}

//set
void Weight_Info::setId(QString id)
{
    this->id = id;
}

void Weight_Info::setWeight(QString weight)
{
    this->weight = weight;
}

void Weight_Info::setBMI(QString bmi)
{
    this->bmi = bmi;
}

void Weight_Info::setStatus(QString status)
{
    this->status = status;
}

void Weight_Info::setTime(QString time)
{
    this->time = time;
}

//get

const QString& Weight_Info::getId() const
{
    return this->id;
}

const QString& Weight_Info::getWeight() const
{
    return this->weight;
}

const QString& Weight_Info::getBMI() const
{
    return this->bmi;
}

const QString& Weight_Info::getStatus() const
{
    return this->status;
}

const QString& Weight_Info::getTime() const
{
    return this->time;
}

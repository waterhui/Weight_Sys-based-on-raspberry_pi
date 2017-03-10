#ifndef WEIGHT_INFO_H
#define WEIGHT_INFO_H

#include <QString>

class Weight_Info
{
public:
    Weight_Info();

    void setId(QString);
    void setWeight(QString);
    void setBMI(QString);
    void setStatus(QString);
    void setTime(QString);

    const QString& getId() const;
    const QString& getWeight() const;
    const QString& getBMI() const;
    const QString& getStatus() const;
    const QString& getTime() const;

private:
    QString id;
    QString weight;
    QString bmi;
    QString status;
    QString time;
};

#endif // WEIGHT_INFO_H

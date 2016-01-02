#ifndef MOTOR_H
#define MOTOR_H

#include "actuator.h"
#include <QList>

class Motor : public Actuator
{
    Q_OBJECT
public:
    Motor(QString _name);
    void setSP(double rpm);
    void setKp(double kp);
    void setTi(double ti);
    void setTd(double td);
    void setDir(double dir);
};

#endif // MOTOR_H

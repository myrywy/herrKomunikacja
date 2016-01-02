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
};

#endif // MOTOR_H

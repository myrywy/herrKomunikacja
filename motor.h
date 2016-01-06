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
    void setCV(double cv);
    void setPid(bool active=true);
    void setKp(double kp);
    void setTi(double ti);
    void setTd(double td);
    void setDir(double dir);
    void toggleDir();
};

#endif // MOTOR_H

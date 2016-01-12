#ifndef SERVO_H
#define SERVO_H

#include "actuator.h"

class Servo : public Actuator
{
    Q_OBJECT
public:
    explicit Servo(QString _name);
    void setVelocity(double v);
    void setAngle(double a);
    int getAngle();
private:
    double velocity;
    int refreshRate;
    QTimer* timer;
    const int maxAngle;
signals:

public slots:
    void refreshAngle();
};

#endif // SERVO_H

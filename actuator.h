#ifndef ACTUATOR_H
#define ACTUATOR_H

#include "sensor.h"

class Actuator : public Sensor
{
    Q_OBJECT
public:
    Actuator(QString _name);
    virtual ~Actuator();

    //void set(double value);
    void set(QList<double> _values);

};

#endif // ACTUATOR_H

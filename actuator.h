#ifndef ACTUATOR_H
#define ACTUATOR_H

#include "sensor.h"

class Actuator : public Sensor
{

public:
    Actuator(QString _name);
    virtual ~Actuator();

    void set(double value);

};

#endif // ACTUATOR_H

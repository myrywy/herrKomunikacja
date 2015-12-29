#ifndef BATTERY_H
#define BATTERY_H

#include "sensor.h"

class Battery : public Sensor
{
public:
    Battery(QString _name);
    virtual void updateDB();
};

#endif // BATTERY_H

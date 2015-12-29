#ifndef BATTERY_H
#define BATTERY_H

#include "sensor.h"

class Battery : public Sensor
{
public:
    Battery();
    virtual void updateDB();
};

#endif // BATTERY_H

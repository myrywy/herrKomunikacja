#ifndef FLOOR_H
#define FLOOR_H

#include "sensor.h"

class Floor : public Sensor
{
    Q_OBJECT
public:
    Floor(QString _name);
    virtual void updateDB();
};

#endif // FLOOR_H

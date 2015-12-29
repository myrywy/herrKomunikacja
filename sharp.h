#ifndef SHARP_H
#define SHARP_H

#include "sensor.h"

class Sharp : public Sensor
{
    Q_OBJECT
public:
    Sharp(QString _name);
    virtual void updateDB();
};

#endif // SHARP_H

#ifndef SONAR_H
#define SONAR_H

#include "sensor.h"

class Sonar : public Sensor
{
    Q_OBJECT
public:
    Sonar(QString _name);
    virtual void updateDB();
};

#endif // SONAR_H

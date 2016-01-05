#ifndef SENSOR_H
#define SENSOR_H

#include "peryferium.h"
#include <functional>
#include <QThread>
#include <QCoreApplication>
#include <QTimer>
//class Peryferium;
class Sensor : public Peryferium
{
    Q_OBJECT
public:
    Sensor(QString _name);
    virtual ~Sensor();

    virtual void updateDB(){;}
    void autoMeasure(int msRate = 50);
    QList<double> measure();
    QList<double> read();
    void setValues(QList<double> _values);

    QList<double> getValues() const;

protected:
    bool waitForValue;
    QList<double> values;
    QTimer* timer;
    QTimer* autoTimer;

protected slots:
    void timeoutHandler();
    void repeatMeasurement();
};
#endif // SENSOR_H

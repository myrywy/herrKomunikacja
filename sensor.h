#ifndef SENSOR_H
#define SENSOR_H

#include "peryferium.h"
#include <functional>
#include <QThread>
#include <QCoreApplication>
#include <QTimer>
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
    void setCallback(std::function<void(Sensor*)> f);
    std::function<void(Sensor*)> callbackFunction;
    void setValues(QList<double> _values);

private:
    bool waitForValue;
    QList<double> values;
    QTimer* timer;

private slots:
    void timeoutHandler();
};
#endif // SENSOR_H

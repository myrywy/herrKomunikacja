#include "sensor.h"
#include <QDebug>

Sensor::Sensor(QString _name)
    :Peryferium(_name){
    waitForValue=false;
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),SLOT(timeoutHandler()));
    timer->setInterval(3000);
    autoTimer = new QTimer(this);
    connect(autoTimer,SIGNAL(timeout()),SLOT(repeatMeasurement()));
}

Sensor::~Sensor(){

}

void Sensor::autoMeasure(int msRate){
    qDebug() << "auto " << msRate;
    autoTimer->setInterval(msRate);
    if(msRate>0){
        autoTimer->start();
    }else{
        autoTimer->stop();
    }
}

QList<double> Sensor::measure(){
    waitForValue=true;
    timer->start();
    sendMessage(GET);
    while(waitForValue){
        QCoreApplication::processEvents();
    }
    return  values;
}


QList<double> Sensor::read(){

    return  values;
}


void Sensor::setCallback(std::function< void(Sensor *)> f){
   callbackFunction=std::function< void(Sensor *)>(f);
}

void Sensor::setValues(QList<double> _values)
{
    //qDebug()<<"setValues";
    waitForValue=false;
    //qDebug()<<callbackFunction;
    if(callbackFunction){
        callbackFunction(this);
    }
    //updateDB();
    values=_values;
}

QList<double> Sensor::getValues() const
{
    return values;
}

void Sensor::timeoutHandler()
{
    waitForValue=false;
    timer->stop();
}

void Sensor::repeatMeasurement()
{
    sendMessage(GET);
}

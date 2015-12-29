#include "sensor.h"
#include <QDebug>

Sensor::Sensor(QString _name)
    :Peryferium(_name){
    waitForValue=false;
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),SLOT(timeoutHandler()));
    timer->setInterval(3000);
}

Sensor::~Sensor(){

}

void Sensor::autoMeasure(int msRate){

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

}

void Sensor::setValues(QList<double> _values)
{
    qDebug()<<"setValues";
    waitForValue=false;
    if(callbackFunction){
        callbackFunction(this);
    }
    values=_values;
}

void Sensor::timeoutHandler()
{
    waitForValue=false;
    timer->stop();
}

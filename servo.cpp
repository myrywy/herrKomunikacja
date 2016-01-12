#include "servo.h"

Servo::Servo(QString _name) : Actuator(_name),
    maxAngle(180)
{
    timer=new QTimer;
    connect(timer,SIGNAL(timeout()),this,SLOT(refreshAngle()));
}

void Servo::setVelocity(double v)
{
    velocity=v;
    refreshRate=1000;
    timer->setInterval(refreshRate);
    if(v!=0){
        timer->start();
    }else{
        timer->stop();
    }
}

void Servo::setAngle(double a)
{
    QList<double> tmpValues;
    tmpValues.append(a);
    set(tmpValues);
}

int Servo::getAngle()
{
    if(getValues().size()==0){
        return -1;
    }
    return getValues()[0];
}

void Servo::refreshAngle()
{
    if(!getValues().size()){
        setAngle(0);
    }
    double currentAngle=getValues()[0];
    if((velocity>0 && currentAngle>=maxAngle)
            || (velocity<0 && currentAngle<=0) ){
        return;
    }
    setAngle(int(currentAngle+velocity*refreshRate/1000));
}

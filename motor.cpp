#include "motor.h"

Motor::Motor(QString _name)
    :Actuator(_name)
{
    for(int i=0;i<9;i++){
        values.append(0);
    }
}

void Motor::setSP(double rpm)
{
    if(values.size()==0){
        throw -1;
    }
    values[0]=rpm;
    QList<double> msgValues;
    msgValues.append(1);
    msgValues.append(rpm);
    qDebug() << "Set SP " << rpm;
    sendMessage(msgValues,SET);
}

void Motor::setKp(double kp)
{

    if(values.size()==0){
        throw -1;
    }
    values[5]=kp;
    QList<double> msgValues;
    msgValues.append(4);
    msgValues.append(kp);
    sendMessage(msgValues,SET);
}

void Motor::setTi(double ti)
{
    if(values.size()==0){
        throw -1;
    }
    values[6]=ti;
    QList<double> msgValues;
    msgValues.append(5);
    msgValues.append(ti);
    sendMessage(msgValues,SET);
}

void Motor::setTd(double td)
{
    if(values.size()==0){
        throw -1;
    }
    values[7]=td;
    QList<double> msgValues;
    msgValues.append(6);
    msgValues.append(td);
    sendMessage(msgValues,SET);
}

void Motor::setDir(double dir)
{
    if(values.size()==0){
        throw -1;
    }
    values[1]=dir;
    QList<double> msgValues;
    msgValues.append(2);
    msgValues.append(dir);
    sendMessage(msgValues,SET);
}




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
    sendMessage(msgValues,SET);
}


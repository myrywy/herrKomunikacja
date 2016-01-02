#include "actuator.h"

Actuator::Actuator(QString _name)
    :Sensor(_name)
{

}

Actuator::~Actuator(){

}

void Actuator::set(QList<double> _values){
    values=_values;
    sendMessage(values,SET);
}

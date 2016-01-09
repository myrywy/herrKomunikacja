#include "condition.h"

Condition::Condition(QObject *parent) : QObject(parent)
{
}

bool Condition::check()
{
    if(checkFunction){
        return checkFunction();
    }else{
        return false;
    }
}

std::function<bool ()> Condition::getCheckFunction() const
{
    return checkFunction;
}

void Condition::setCheckFunction(const std::function<bool ()> &value)
{
    checkFunction = value;
}


#include "navigator.h"
#include "debuginfo.h"

Navigator::Navigator(QObject *parent) : QObject(parent)
{
    for(int i=0;i<int(DirectionsNumber);i++){
        blocked.append(new Condition(this));
        free.append(new Condition(this));
    }
    for(int i=0;i<2;i++){
        floor.append(new Condition(this));
        noFloor.append(new Condition(this));
    }
}

Navigator::~Navigator()
{

}

State* Navigator::getCurrentState() const
{
    return currentState;
}

void Navigator::setCurrentState(State *value)
{
    if(true || value!=currentState){
        stateInfo(value->getName());
        currentState = value;
        emit stateChanged(currentState);
        emit stateChanged(currentState->getMotorsState());
    }
}

void Navigator::newConditions()
{
    setCurrentState(currentState->newState());
}


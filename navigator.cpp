#include "navigator.h"

Navigator::Navigator(QObject *parent) : QObject(parent)
{

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
    if(value!=currentState){
        currentState = value;
        emit stateChanged(currentState);
    }
    for(int i=0;i<int(DirectionsNumber);i++){
        blocked.append(new Condition(this));
        free.append(new Condition(this));
    }
    for(int i=0;i<2;i++){
        floor.append(new Condition(this));
        noFloor.append(new Condition(this));
    }
}

void Navigator::newConditions()
{
    setCurrentState(currentState->newState());
}


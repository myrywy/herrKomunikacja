#include "state.h"
#include <QtGlobal>
#include <QTime>

State::State(QObject *parent) : QObject(parent)
{
    qsrand(QTime::currentTime().msec());
}

void State::addTransition(Transition t)
{
    transitions.append(t);
}


State* State::newState()
{
    QList<State*> avilable;
    for(Transition& transition : transitions){
        State* tmp=transition.nextState;
        if(tmp){
            avilable.append(tmp);
        }
    }
    int n=avilable.size();
    if(n==0){
        return this;
    }
    if(n==1){
        return avilable[0];
    }
    int choice=qrand()%n;
    return avilable[choice];
}

MotorsState State::getMotorsState() const
{
    return motorsState;
}

void State::setMotorsState(const MotorsState &value)
{
    motorsState = value;
}

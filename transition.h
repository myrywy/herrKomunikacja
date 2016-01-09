#ifndef TRANSITION_H
#define TRANSITION_H

#include <QObject>
#include "condition.h"

class State;

class Transition
{
public:
    explicit Transition(State* _nextState);
    State* nextState;
    QList<Condition*> guards;
    Transition& operator<<(Condition& newGuard);
    Transition& operator<<(Condition* newGuard);
    void addGuard(Condition* newGuard);
    //State* checkTransition(Situation situation);
    State* checkTransition();
};

#endif // TRANSITION_H

#include "transition.h"

Transition::Transition(State *_nextState)
    :nextState(_nextState)
{

}

Transition &Transition::operator<<(Condition& newGuard)
{
    addGuard(&newGuard);
    return *this;
}

Transition &Transition::operator<<(Condition *newGuard)
{
    addGuard(newGuard);
    return *this;
}

void Transition::addGuard(Condition* newGuard)
{
    guards.append(newGuard);
}

/*State *Transition::checkTransition(Situation situation)
{
    for(Condition* condition : guards){
        if(!condition->check()){
            return nullptr;
        }
    }
    return nextState;
}*/

State *Transition::checkTransition()
{
    for(Condition* condition : guards){
        if(!condition->check()){
            return nullptr;
        }
    }
    return nextState;
}

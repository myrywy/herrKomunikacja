#include "avoidcollisionalgorithm.h"

AvoidCollisionAlgorithm::AvoidCollisionAlgorithm(QObject *parent) : Navigator(parent)
{
    cofanie=new State(this);
    naWprost=new State(this);
    wLewo=new State(this);
    wPrawo=new State(this);
    stop=new State(this);

    cofanie->setMotorsState(TYL);
    naWprost->setMotorsState(NA_WPROST);
    wLewo->setMotorsState(W_LEWO);
    wPrawo->setMotorsState(W_PRAWO);
    stop->setMotorsState(STOP);

    cofanie->addTransition(Transition(wPrawo)<<free[RIGHT]<<free[FRONT_LEFT]);
    cofanie->addTransition(Transition(wLewo)<<free[LEFT]<<free[FRONT_RIGHT]);
    cofanie->addTransition(Transition(stop)<<free[EXT_BACK]);

    naWprost->addTransition(Transition(wLewo)<<blocked[FRONT]<<blocked[RIGHT]);
    naWprost->addTransition(Transition(wPrawo)<<blocked[FRONT]<<blocked[LEFT]);
    naWprost->addTransition(Transition(cofanie)<<blocked[EXT_FRONT]<<blocked[RIGHT]<<blocked[RIGHT]);
    naWprost->addTransition(Transition(stop)<<blocked[FRONT]<<blocked[REAR]<<blocked[LEFT]<<blocked[RIGHT]);

    wPrawo->addTransition(Transition(naWprost)<<free[FRONT]);
    wPrawo->addTransition(Transition(cofanie)<<blocked[EXT_FRONT]<<blocked[RIGHT]<<blocked[RIGHT]);
    wPrawo->addTransition(Transition(stop)<<blocked[FRONT]<<blocked[REAR]<<blocked[LEFT]<<blocked[RIGHT]);

    wLewo->addTransition(Transition(naWprost)<<free[FRONT]);
    wLewo->addTransition(Transition(cofanie)<<blocked[EXT_FRONT]<<blocked[RIGHT]<<blocked[RIGHT]);
    wLewo->addTransition(Transition(stop)<<blocked[FRONT]<<blocked[REAR]<<blocked[LEFT]<<blocked[RIGHT]);

    stop->addTransition(Transition(naWprost)<<free[FRONT]);
    stop->addTransition(Transition(wPrawo)<<free[RIGHT]);
    stop->addTransition(Transition(wLewo)<<free[LEFT]);
    stop->addTransition(Transition(cofanie)<<free[EXT_BACK]);

    currentState=stop;
}


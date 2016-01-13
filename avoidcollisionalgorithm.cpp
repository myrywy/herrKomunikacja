#include "avoidcollisionalgorithm.h"

AvoidCollisionAlgorithm::AvoidCollisionAlgorithm(QObject *parent) : Navigator(parent)
{
    cofanie=new State("cofanie",this);
    naWprost=new State("naWprost",this);
    wLewo=new State("wLewo",this);
    wPrawo=new State("wPrawo",this);
    stop=new State("stop",this);

    cofanie->setMotorsState(TYL);
    naWprost->setMotorsState(NA_WPROST);
    wLewo->setMotorsState(W_LEWO);
    wPrawo->setMotorsState(W_PRAWO);
    stop->setMotorsState(STOP);

    cofanie->addTransition(Transition(wPrawo)<<free[RIGHT]<<free[FRONT_RIGHT]);
    cofanie->addTransition(Transition(wLewo)<<free[LEFT]<<free[FRONT_LEFT]);
    cofanie->addTransition(Transition(stop)<<free[EXT_BACK]);

    naWprost->addTransition(Transition(wLewo)<<blocked[FRONT]<<blocked[RIGHT]);
    naWprost->addTransition(Transition(wPrawo)<<blocked[FRONT]<<blocked[LEFT]);
    naWprost->addTransition(Transition(cofanie)<<blocked[EXT_FRONT]<<blocked[RIGHT]<<blocked[RIGHT]);
    naWprost->addTransition(Transition(stop)<<blocked[FRONT]<<blocked[REAR]<<blocked[LEFT]<<blocked[RIGHT]);

    wPrawo->addTransition(Transition(naWprost)<<free[FRONT]);
    wPrawo->addTransition(Transition(cofanie)<<blocked[EXT_FRONT]<<blocked[RIGHT]<<blocked[RIGHT]);
    wPrawo->addTransition(Transition(stop)<<blocked[FRONT]<<blocked[REAR]<<blocked[LEFT]<<blocked[RIGHT]);

    wLewo->addTransition(Transition(naWprost)<<free[FRONT]);
    wLewo->addTransition(Transition(cofanie)<<blocked[EXT_FRONT]<<blocked[LEFT]<<blocked[RIGHT]);
    wLewo->addTransition(Transition(stop)<<blocked[FRONT]<<blocked[REAR]<<blocked[LEFT]<<blocked[RIGHT]);

    stop->addTransition(Transition(naWprost)<<free[FRONT]);
    stop->addTransition(Transition(wPrawo)<<free[RIGHT]<<blocked[FRONT]);
    stop->addTransition(Transition(wLewo)<<free[LEFT]<<blocked[FRONT]);
    stop->addTransition(Transition(cofanie)<<free[EXT_BACK]<<blocked[FRONT]);

    currentState=stop;
}


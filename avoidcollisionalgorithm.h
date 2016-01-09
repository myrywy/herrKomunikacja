#ifndef AVOIDCOLLISIONALGORITHM_H
#define AVOIDCOLLISIONALGORITHM_H

#include "navigator.h"

class AvoidCollisionAlgorithm : public Navigator
{
    Q_OBJECT
public:
    explicit AvoidCollisionAlgorithm(QObject *parent = 0);
    State* cofanie;
    State* wLewo;
    State* wPrawo;
    State* stop;
    State* naWprost;
};

#endif // AVOIDCOLLISIONALGORITHM_H

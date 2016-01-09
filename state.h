#ifndef STATE_H
#define STATE_H

#include <QObject>
#include "transition.h"
#include "motorstate.h"

class State : public QObject
{
    Q_OBJECT
public:
    explicit State(QObject *parent = 0);
    void addTransition(Transition t);
    State* newState();
    MotorsState getMotorsState() const;
    void setMotorsState(const MotorsState &value);

protected:
    QList<Transition> transitions;
    MotorsState motorsState;
};



#endif // STATE_H

#ifndef NAVIGATOR_H
#define NAVIGATOR_H

#include <QObject>
#include "state.h"
#include "enums.h"

class Navigator : public QObject
{
    Q_OBJECT
public:
    explicit Navigator(QObject *parent = 0);
    virtual ~Navigator();
    State *getCurrentState() const;
    void setCurrentState(State *value);

    QList<Condition*> blocked;
    QList<Condition*> free;
    QList<Condition*> floor;
    QList<Condition*> noFloor;
protected:
    State* currentState;
signals:
    void stateChanged(State*);
    void stateChanged(MotorsState);

public slots:
    void newConditions();
};

#endif // NAVIGATOR_H

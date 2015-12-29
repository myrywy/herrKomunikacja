#ifndef MAINTHREAD_H
#define MAINTHREAD_H

#include <QObject>
#include <QThread>
#include "robot.h"

class MainThread : public QThread
{
public:
    MainThread();
    Robot* robot;
    virtual void run();
};

#endif // MAINTHREAD_H

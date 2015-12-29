#include "mainthread.h"

MainThread::MainThread()
{
    robot = new Robot();
    robot->moveToThread(this);
}

void MainThread::run()
{
    qDebug() << "run";
}

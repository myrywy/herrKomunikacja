#include <QCoreApplication>
#include <QThread>
#include "mainthread.h"
#include "robot.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    //MainThread th;
    //th.start();
    Robot* robot = new Robot();
    return a.exec();
}

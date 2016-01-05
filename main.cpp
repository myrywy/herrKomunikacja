#include <QCoreApplication>
#include <QThread>
//#include "mainthread.h"
#include "robot.h"
#include "debuginfo.h"
#include <iostream>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    //MainThread th;
    //th.start();
    debugInfo();
    Robot* robot = new Robot();
    robot->setParent(&a);
    try{
        return a.exec();
    }catch(...){
        std::cout<<"Nieznany błąd.";
        std::cout.flush();
        delete robot;
    }
}

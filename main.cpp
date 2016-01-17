#include <QCoreApplication>
#include <QThread>
//#include "mainthread.h"
#include "robot.h"
#include "debuginfo.h"
#include "imageprocessor.h"
#include <iostream>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    //MainThread th;
    //th.start();
    debugInfo();
    Robot* robot = new Robot();
    robot->setParent(&a);
    ImageProcessor* imgPr=new ImageProcessor(0,10,4,&a);
    try{
        return a.exec();
    }catch(...){
        std::cout<<"Nieznany błąd.";
        std::cout.flush();
        delete robot;
        delete imgPr;
    }
}

#include "comport.h"
#include <QDebug>
#include <QTime>

/*ComPort::ComPort()
{
    waiting=false;
    timer = new QTimer(this);
    timer->setTimerType(Qt::PreciseTimer);
    timer->setInterval(300);
    connect(timer,SIGNAL(timeout()),this,SLOT(goOn()));
    timer->setSingleShot(true);
}*/

ComPort::ComPort(QString _portName)
    :QSerialPort(_portName)
{
    waiting=false;
    timer = new QTimer(this);
    maxWaitTime=300;
    timer->setInterval(maxWaitTime);
    connect(timer,SIGNAL(timeout()),this,SLOT(waitingTimeout()));
    connect(timer,SIGNAL(timeout()),this,SLOT(goOn()));
    timer->setSingleShot(true);
}

qint64 ComPort::writeData(const char * data, qint64 maxSize)
{
    const QByteArray dataArray(data);
    if(QString(dataArray).contains("motor left-set")){
        qDebug()<<"motor left-set";
    }
    if(queue.size()>20){
        resetQueue();
    }
    //qWarning() << "enqueued: " << queue.size();
    qInfo() << "enqueued: " << queue.size();
    if(waiting){
        qInfo() << "enqueue: " << data;
        queue.enqueue(dataArray);
    }else{
        //qDebug() << QTime::currentTime().toString();
        qInfo() << "<-" << data;
        qInfo() << QTime::currentTime().toString("hh:mm:ss,zzz");
        QSerialPort::writeData(data,maxSize);
        wait(true);
    }
    //qDebug() << "to write " << dataArray.size();
    return dataArray.size();
}

void ComPort::resetQueue()
{
    timer->stop();
    timer->setInterval(maxWaitTime);
    queue.clear();
    wait(false);
}

void ComPort::wait(bool _wait)
{
    waiting=_wait;
    if(_wait==true){
        timer->start();
    }else{
        timer->stop();
        qInfo()<< "Remaining time" << timer->remainingTime();
    }
}

void ComPort::goOn()
{
    //qDebug() << "go on";
    if(queue.size()!=0){
        QByteArray arr=queue.dequeue();
        qInfo() << "<-" << arr;
        qInfo() << QTime::currentTime().toString("hh:mm:ss,zzz");
        QSerialPort::writeData(arr.data(), arr.size());
        wait(true);
    }else{
        wait(false);
    }
}

void ComPort::waitingTimeout()
{
    qInfo() << "Timeout!";
    qInfo() << QTime::currentTime().toString("hh:mm:ss,zzz");
    //qWarning() << "Timeout!";
    //qWarning() << QTime::currentTime().toString("hh:mm:ss,zzz");

}

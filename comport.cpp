#include "comport.h"
#include <QDebug>
#include <QTime>

ComPort::ComPort()
{
    waiting=false;
    timer = new QTimer(this);
    timer->setInterval(100);
    connect(timer,SIGNAL(timeout()),this,SLOT(goOn()));
    timer->setSingleShot(true);
}

ComPort::ComPort(QString _portName)
    :QSerialPort(_portName)
{
    waiting=false;
    timer = new QTimer(this);
    timer->setInterval(100);
    connect(timer,SIGNAL(timeout()),this,SLOT(goOn()));
    timer->setSingleShot(true);
}

qint64 ComPort::writeData(const char * data, qint64 maxSize)
{
    const QByteArray dataArray(data);
    if(waiting){
        queue.enqueue(dataArray);
    }else{
        qDebug() << QTime::currentTime().toString();
        QSerialPort::writeData(data,maxSize);
        wait();
    }
    //qDebug() << "to write " << dataArray.size();
    return dataArray.size();
}

void ComPort::wait(bool _wait)
{
    waiting=_wait;
    if(_wait==true){
        timer->start();
    }
}

void ComPort::goOn()
{
    qDebug() << "go on";
    if(queue.size()!=0){
        QByteArray arr=queue.dequeue();
        QSerialPort::writeData(arr.data(), arr.size());
        wait();
    }else{
        wait(false);
    }
}

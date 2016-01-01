#include "comport.h"
#include <QDebug>

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
    if(!waiting){
        qDebug() << "nie czekaj";
        wait();
        QSerialPort::writeData(dataArray.data(),maxSize);
        timer->start();
    }else{
        qDebug() << "czekaj " << queue.size();
        queue.enqueue(dataArray);
    }
    //qDebug() << "to write " << dataArray.size();
    return dataArray.size();
}

void ComPort::wait()
{
    waiting=true;
}

void ComPort::goOn()
{
    qDebug() << "go on";
    waiting=false;
    if(queue.size()!=0){
        write(queue.dequeue());
    }
}

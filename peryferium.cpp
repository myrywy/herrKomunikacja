#include "peryferium.h"
#include <QDebug>

Peryferium::Peryferium(QString _name)
    :name(_name)
{
    if(theParser){
        theParser->registerPeryferium(this);
        port=theParser->getPort();
    }
}

Peryferium::~Peryferium(){

}

QString Peryferium::getName(){

    return name;
}

void Peryferium::sendMessage(MessageType type){
    Command msg;
    msg<<name;
    msg<<typeToString(type);
    if(port){
        QByteArray array=msg.byteArray();
        qDebug()<<array;
        port->write(array);
    }
}

void Peryferium::sendMessage(double value, MessageType type){
    Command msg;
    msg<<name;
    msg<<typeToString(type);
    msg<<value;
    if(port){
        port->write(msg.byteArray());
    }
}

void Peryferium::sendMessage(QList<double> values, MessageType type){
    Command msg;
    msg<<name;
    msg<<typeToString(type);
    msg<<values;
    if(port){
        port->write(msg.byteArray());
    }
}

void Peryferium::setName(QString newVal){

         name = newVal;
}

QString Peryferium::typeToString(MessageType type)
{
    switch(type){
    case GET:
        return QString("get");
    case SET:
        return QString("set");
    case MEASUREMENT:
        return QString("measurement");
    case ACK:
        return QString("ack");
    }
}

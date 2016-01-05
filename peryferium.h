#ifndef PERYFERIUM_H
#define PERYFERIUM_H

#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <functional>
//#include  "parser.h"
#include "command.h"

enum MessageType{
    GET,
    SET,
    MEASUREMENT,
    ACK
};



class Peryferium : public QObject
{
    Q_OBJECT
public:
    Peryferium(QString _name);
    virtual ~Peryferium();
    QSerialPort *port;

    QString getName();
    void sendMessage(MessageType type);
    void sendMessage(double value, MessageType type);
    void sendMessage(QList<double> values, MessageType type);
    void setName(QString newVal);
    static QString typeToString(MessageType type);
    //void setCallback(std::function<void(Peryferium*)> f);
    //std::function<void(Peryferium*)> callback();
    std::function<void (Peryferium *)> getCallbackFunction() const;
    void setCallbackFunction(const std::function<void (Peryferium *)> &value);
    std::function<void(Peryferium*)> callbackFunction;

protected:

    QString name;
    void setValues(QList<double> _values);

};
/*
void Peryferium::setCallback(std::function<(Peryferium *)> f){
    callbackFunction=std::function< void(Sensor *)>(f);
}

std::function<void (Peryferium *)> Peryferium::callback()
{
    return callbackFunction;
}
*/
#endif // PERYFERIUM_H

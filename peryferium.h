#ifndef PERYFERIUM_H
#define PERYFERIUM_H

#include <QObject>
#include <QtSerialPort/QSerialPort>
#include  "parser.h"
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

private:
    QString name;
};

#endif // PERYFERIUM_H

#ifndef PARSER_H
#define PARSER_H

#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <QDebug>
//#include "peryferium.h"

class Peryferium;

class Parser : public QObject
{
    Q_OBJECT
public:
    Parser(QSerialPort* _port);
    void parse(QString message);
    static Parser* parser;
    QByteArray getBuffer();
    static bool registerPeryferium(Peryferium* p);
    static bool unregisterPeryferium(Peryferium* p);
    void addPeryferium(Peryferium* p);
    bool removePeryferium(Peryferium* p);
    Peryferium* findByName(QString name);
    static QList<double> stringToDoubleList(QString str);
    static QString doubleListToString(QList<double> values);
    enum PState{
        WAITING=0,
        IN_PROGRESS=1
    };
    QSerialPort *getPort() const;
    void setPort(QSerialPort *value);

public slots:
    void dataAvilable();
protected:
    void process();
    QByteArray buffer;
    QSerialPort* port;
    int pointer;
    PState state;
    QList<Peryferium*> peryferia;
signals:
    void commandReceived();
};

#define theParser Parser::parser

#endif // PARSER_H

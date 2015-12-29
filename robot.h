#ifndef ROBOT_H
#define ROBOT_H

#include <QtSerialPort/QSerialPort>
#include <QSerialPortInfo>
#include <QTimer>
#include <QDebug>
#include <QTcpServer>
#include <QTcpSocket>
#include <QSharedPointer>
#include <QtSql>
#include <QtSql/QSqlDatabase>
#include "parser.h"
#include "sensor.h"
#include "actuator.h"
#include "sonar.h"
#include "sharp.h"
#include "floor.h"

class Robot : public QObject
{
    Q_OBJECT
public:
    Robot();
    ~Robot();
    static QSqlDatabase* db;
protected:
    //Sensor* sonar;
    Sonar* sonar;
    Sharp* sharp;
    Floor* floor;
    Actuator* motorLeft;
    QTimer* timer;
    QSerialPort* port;
    QTcpServer* tcpServer;
    QList< QSharedPointer<QTcpSocket> > sockets;
private slots:
    void timerHandler();
    void newConnectionHandler();
    void tcpReadyReadHandler();
    void tcpDisconnectedHandler();
};



#endif // ROBOT_H

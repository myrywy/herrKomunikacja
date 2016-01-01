#ifndef ROBOT_H
#define ROBOT_H

#include "comport.h"
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
#include <QVector>

enum Direction{
   FRONT=0,
    FRONT_LEFT,
    FRONT_RIGHT,
    LEFT,
    RIGHT,
    REAR,
    REAR_LEFT,
    REAR_RIGHT
};

class Robot : public QObject
{
    Q_OBJECT
public:
    Robot();
    ~Robot();
    static QSqlDatabase* db;
    void updatePosition(Sensor* s);
protected:
    QVector<double> position;
    //Sensor* sonar;
    Sonar* sonar;
    Sharp* sharp;
    Floor* floor;
    Actuator* motorLeft;
    Actuator* motorRight;
    QTimer* timer;
    ComPort* port;
    QTcpServer* tcpServer;
    QList< QSharedPointer<QTcpSocket> > sockets;
private slots:
    void timerHandler();
    void newConnectionHandler();
    void tcpReadyReadHandler();
    void tcpDisconnectedHandler();
};



#endif // ROBOT_H

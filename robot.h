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
#include "motor.h"
#include "velocity.h"
#include <QVector>

enum Direction{
   FRONT=0,
    FRONT_LEFT,
    FRONT_RIGHT,
    LEFT,
    RIGHT,
    REAR,
    REAR_LEFT,
    REAR_RIGHT,
    EXT_FRONT,
    EXT_BACK
};

enum State{
    COFANIE,
    ZAWRACANIE,
    NA_WPROST,
    W_LEWO,
    W_PRAWO,
    STOP
};

class Robot : public QObject
{
    Q_OBJECT
public:
    Robot();
    ~Robot();
    static QSqlDatabase* db;
    void updatePosition(Sensor* s);
    void checkFloor(Sensor* s);
    State getState() const;

protected:
    void setState(const State &value);
    QVector<double> position;
    State state;
    Velocity velocity;
    //Sensor* sonar;
    Sonar* sonar;
    Sharp* sharp;
    Floor* rearFloor;
    Floor* frontFloor;
    Motor* motorLeft;
    Motor* motorRight;
    QTimer* timer;
    ComPort* port;
    QTcpServer* tcpServer;
    QList< QSharedPointer<QTcpSocket> > sockets;
    bool checkObstacle(Direction dir, double max=20,double min=0);

    void checkState();
    void setVelocity(Velocity v);

protected slots:
    void timerHandler();
    void newConnectionHandler();
    void tcpReadyReadHandler();
    void tcpDisconnectedHandler();
};



#endif // ROBOT_H

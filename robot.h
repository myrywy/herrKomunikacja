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
//#include "parser.h"
#include "sensor.h"
#include "actuator.h"
#include "sonar.h"
#include "sharp.h"
#include "floor.h"
#include "battery.h"
#include "motor.h"
#include "velocity.h"
#include <QQueue>
#include <QVector>

#include "enums.h"

enum Control{
    AUTO,
    HAND
};

class TcpServer;

class Navigator;

class Robot : public QObject
{
    Q_OBJECT
public:
    Robot();
    ~Robot();
    static QSqlDatabase* db;
    void updatePosition(Sensor* s);
    void checkFloor(Sensor* s);
    void checkVoltage(Sensor* s);
    MotorsState getState() const;

    Control getControl() const;
    void setControl(const Control &value);

protected:
    Navigator* navigator;
    void setupNavigator();
    QVector<double> position;
    QQueue<double> voltage;
    MotorsState state;
    Control control;
    Velocity velocity;
    //Sensor* sonar;
    Peryferium* system;
    Sonar* sonar;
    Sharp* sharp;
    Floor* rearFloor;
    Floor* frontFloor;
    Battery* battery;
    Motor* motorLeft;
    Motor* motorRight;
    QTimer* timer;
    ComPort* port;
    TcpServer* server;
    bool checkObstacle(Direction dir, double max=15,double min=0);
    void setupMotors();
    void checkState();
    void setVelocity(Velocity v);
    void systemReset();

protected slots:
    void timerHandler();
public slots:
    void setState(const MotorsState &value);

};



#endif // ROBOT_H

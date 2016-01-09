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
    STOP,
    ROZRUCH,
    TYL,
    AUTO_ON
};

enum Control{
    AUTO,
    HAND
};

class TcpServer;

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
    State getState() const;

    Control getControl() const;
    void setControl(const Control &value);
    void setState(const State &value);

protected:
    QVector<double> position;
    QQueue<double> voltage;
    State state;
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
};



#endif // ROBOT_H

#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QTcpServer>
//#include "tcpsocket.h"
#include <QTcpSocket>
#include "robot.h"

class TcpServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit TcpServer(QObject *parent = 0);
    Robot *getRobot() const;
    void setRobot(Robot *value);

protected:
    QList< QSharedPointer<QTcpSocket> > sockets;
    int checkCommand(QString msg);
    int checkCameraCommand(QString msg);
    QVector<QString> commands;
    QVector<QString> cameraCommands;
    Robot* robot;
signals:

public slots:
    void newConnectionHandler();
    void tcpReadyReadHandler(QTcpSocket *socket);
    void tcpDisconnectedHandler();
};

#endif // TCPSERVER_H

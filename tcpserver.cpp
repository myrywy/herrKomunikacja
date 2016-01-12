#include "tcpserver.h"
//#include "connector.h"
#include <QString>
#include <QByteArray>
#include "retransmiter.h"
TcpServer::TcpServer(QObject *parent) : QTcpServer(parent)
{
    //tcpServer = new QTcpServer(this);
    commands=QVector<QString>(10);
    commands[NA_WPROST]="robot_forward";
    commands[W_LEWO]="robot_left";
    commands[W_PRAWO]="robot_right";
    commands[STOP]="robot_stop";
    commands[ROZRUCH]="robot_start";
    commands[TYL]="robot_back";
    commands[AUTO_ON]="auto";

    cameraCommands=QVector<QString>(int(CameraMove::CameraMoveNumber));
    cameraCommands[int(CameraMove::UP)]="camera_up";
    cameraCommands[int(CameraMove::DOWN)]="camera_down";
    cameraCommands[int(CameraMove::RIGHT)]="camera_right";
    cameraCommands[int(CameraMove::LEFT)]="camera_left";
    cameraCommands[int(CameraMove::STOP)]="camera_stop";

    connect(this,SIGNAL(newConnection()),this,SLOT(newConnectionHandler()));
}

int TcpServer::checkCommand(QString msg)
{
    for(int i = 0; i<commands.size(); i++){
        if(commands[i].size()!=0 && msg.contains(commands[i])){
            return i;
        }
    }
    return -1;
}

int TcpServer::checkCameraCommand(QString msg)
{
    for(int i = 0; i<cameraCommands.size(); i++){
        if(cameraCommands[i].size()!=0 && msg.contains(cameraCommands[i])){
            return i;
        }
    }
    return -1;
}

Robot *TcpServer::getRobot() const
{
    return robot;
}

void TcpServer::setRobot(Robot *value)
{
    robot = value;
}


void TcpServer::newConnectionHandler()
{
    /*if(!tcpServer){
        return;
    }*/
    QTcpSocket* newSocket = nextPendingConnection();
    Retransmiter* r=new Retransmiter(newSocket);
    //const char* sig=SIGNAL(readyRead());
    //const char* slo=SLOT(tcpReadyReadHandler(TcpSocket*));
    connect(newSocket,SIGNAL(readyRead()),r,SLOT(reemit()));
    connect(r,SIGNAL(readyRead(QTcpSocket*)),this,SLOT(tcpReadyReadHandler(QTcpSocket*)));
    //connect(newSocket,sig,this,slo);
    qWarning() << "New connection!";
    //connect(newSocket,SIGNAL(disconnected()),newSocket,SLOT(deleteLater()));
}

void TcpServer::tcpReadyReadHandler(QTcpSocket* socket)
{
    qWarning() << "readyRead. ";
    QString bytes=(socket->peek(1000));//readAll();
    if(bytes.size()==0){
        return;
    }
    if(bytes.contains(";")){
        socket->readAll();
    }
    qWarning() << "data: " << bytes;
    //port->write(bytes);
    if(bytes.contains("robot")){
        MotorsState st=MotorsState(checkCommand(bytes));
        if(st!=-1){
            socket->readAll();
            robot->setControl(HAND);
            robot->setState(st);
        }
    }else if(bytes.contains("camera")){
        int ist=(checkCameraCommand(bytes));
        if(ist!=-1){
            CameraMove st=CameraMove(ist);
            socket->readAll();
            robot->camera->setMove(st);
        }
    }
    /*for(QSharedPointer<QTcpSocket> s : sockets){
        QString bytes=(s->peek(1000));//readAll();
        qWarning() << "data: " << bytes;
        //port->write(bytes);
        State st=State(checkCommand(bytes));
        if(st!=-1){
            s->readAll();
            robot->setControl(HAND);
            robot->setState(st);
        }
    }*/
}

void TcpServer::tcpDisconnectedHandler()
{
    QList< QSharedPointer<QTcpSocket> > newListOfSockets;
    for(int i=0;i<sockets.size();i++){
        QSharedPointer<QTcpSocket> s=sockets[i];
        if(!(s->state()==QTcpSocket::UnconnectedState)){
            newListOfSockets.append(sockets[i]);
        }
    }
    sockets=newListOfSockets;
}

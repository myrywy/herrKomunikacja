#include "tcpserver.h"
//#include "connector.h"
#include <QString>
#include <QByteArray>
#include "retransmiter.h"
TcpServer::TcpServer(QObject *parent) : QTcpServer(parent)
{
    //tcpServer = new QTcpServer(this);
    commands=QVector<QString>(7);
    commands[NA_WPROST]="forward";
    commands[W_LEWO]="left";
    commands[W_PRAWO]="right";
    commands[STOP]="stop";
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
    qWarning() << "data: " << bytes;
    //port->write(bytes);
    State st=State(checkCommand(bytes));
    if(st!=-1){
        socket->readAll();
        robot->setControl(HAND);
        robot->setState(st);
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

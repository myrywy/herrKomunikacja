#include "robot.h"

Robot::Robot()
    :port(0),
      position(8)
{
    /*
     *
     * Baza danych
     *
     */
    QSqlDatabase database = QSqlDatabase::addDatabase("QMYSQL","my_sql_db");
    /*db.setHostName("127.0.0.1");
    db.setPort(3306);
    db.setDatabaseName("herring");
    db.setUserName("user1");
    db.setPassword("herring1");*/


    database.setHostName("192.168.0.17");
    database.setPort(3306);
    database.setDatabaseName("herring");
    database.setUserName("root1");
    database.setPassword("admin");

    db=new QSqlDatabase(database);

    if (!db->open())
    {
        qDebug() << "Błąd: nie można się połączyć z bazą!";
    }
    else
    {
        qDebug() << "Nawiązano połączenie z bazą danych.";
    }
    /*
     *
     * UART
     *
     */
    QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();
    for(int i=0; i<ports.size(); i++){
        qDebug() << ports[i].portName();
    }
    if(ports.size()==0){
        qDebug() << "Brak dostepnych portow";
        return;
    }
    //port=new QSerialPort(ports[0].portName());
    port=new ComPort("COM3");
    port->setBaudRate(QSerialPort::Baud9600);
    if(port->open(QIODevice::ReadWrite)){
        qDebug()<< "Port otwarty";
        new Parser(port);
        Parser::parser->setParent(this);
        connect(theParser,SIGNAL(commandReceived()),port,SLOT(goOn()));
    }else{
        qDebug()<< "Nie udało się otworzyć portu";
    }
    /*
     * TCP
     */
    tcpServer = new QTcpServer(this);
    /*
     * Inicjalizacja peryferiów
     */
    qDebug() << "Inicjalizacja sensorów...";
    sonar = new Sonar("sonars");
    sharp = new Sharp("analog sharps");
    sonar->setCallback([&this->position](Sensor* s){
        qDebug() << position.size();
        updatePosition(s);
    });
    sonar->autoMeasure(500);
    sharp->autoMeasure(500);
    qDebug() << "Sensory OK.";
    /*
     * misc, testowe
     */
    //sonar = new Sensor("sonary");
    //position = QVector<double>(8);
    motorLeft = new Actuator("motor left");
    motorRight = new Actuator("motor right");
    timer=new QTimer(this);
    timer->setInterval(5000);
    timer->start();
    connect(timer,SIGNAL(timeout()),SLOT(timerHandler()));
}

Robot::~Robot()
{
    qDebug() << "fin.";
    if(port){
        port->close();
    }
}

void Robot::updatePosition(Sensor *s)
{
    if(s->getName()=="sonars"){
        QList<double> v=s->getValues();
        position[FRONT]=v[0];
        position[FRONT_LEFT]=v[1];
        position[FRONT_RIGHT]=v[2];
        position[REAR]=v[3];
        position[REAR_RIGHT]=v[4];
        position[REAR_LEFT]=v[5];
        qDebug() << "F " << position[FRONT]
                    << "FL " << position[FRONT_LEFT]
                    << "FR " << position[FRONT_RIGHT]
                    << "R " << position[REAR]
                    << "RL " << position[REAR_LEFT]
                    << "RR " << position[REAR_RIGHT];
    }else if(s->getName()=="analog sharps"){
        ;
    }
}

void Robot::timerHandler()
{
    /*if(Parser::parser){
        qDebug() << "Buff data";
        qDebug() << QString(Parser::parser->getBuffer());
    }*/
    /*if(sonar){
        qDebug() << "sonary: ";
        qDebug() << Parser::doubleListToString(sonar->read());
    }*/
    /*if(motorLeft){
            qDebug() << "Motor left: ";
            qDebug() << Parser::doubleListToString(sonar->measure());
    }*/
    /*if(motorLeft){
                qDebug() << "Motor left: ";
                qDebug() << Parser::doubleListToString(motorLeft->measure());
    }*/
    qDebug() << "still alive";
}

void Robot::newConnectionHandler()
{
    if(!tcpServer){
        return;
    }
    QTcpSocket* newSocket = tcpServer->nextPendingConnection();
    connect(newSocket,SIGNAL(readyRead()),this,SLOT(tcpReadyReadHandler()));
    //connect(newSocket,SIGNAL(disconnected()),newSocket,SLOT(deleteLater()));
}

void Robot::tcpReadyReadHandler()
{
    for(QSharedPointer<QTcpSocket> s : sockets){
        QByteArray bytes=s->readAll();
        port->write(bytes);
    }
}

void Robot::tcpDisconnectedHandler()
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

QSqlDatabase* Robot::db=nullptr;

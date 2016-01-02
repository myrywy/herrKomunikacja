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
    port=new ComPort("COM4");
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
    frontFloor = new Floor("floor front");
    rearFloor = new Floor("floor back");
    auto delegatUpdatePosition=[&](Sensor* s)mutable{
        qDebug() << position.size()  << "ddd";
        updatePosition(s);
    };
    auto delegateCheckFloor=[&](Sensor* s)mutable{
        qDebug() << position.size()  << "ddd";
        checkFloor(s);
    };
    sharp->setCallback(delegatUpdatePosition);
    sonar->setCallback(delegatUpdatePosition);
    frontFloor->setCallback(delegateCheckFloor);
    rearFloor->setCallback(delegateCheckFloor);
    sonar->autoMeasure(1000);
    sharp->autoMeasure(1000);
    frontFloor->autoMeasure(1000);
    rearFloor->autoMeasure(1000);
    qDebug() << "Sensory OK.";
    motorLeft = new Motor("motor left");
    motorRight = new Motor("motor right");
    /*
     * misc, testowe
     */
    //sonar = new Sensor("sonary");
    //position = QVector<double>(8);
    timer=new QTimer(this);
    timer->setInterval(500);
    timer->start();
    connect(timer,SIGNAL(timeout()),SLOT(timerHandler()));
    state=NA_WPROST;
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
        position[REAR_LEFT]=v[0];
        position[REAR]=v[1];
        position[REAR_RIGHT]=v[2];
        position[FRONT_LEFT]=v[3];
        position[FRONT_RIGHT]=v[4];
        //position[FRONT]=v[5]; //Ten sonar nie działa.
        /*qDebug() << "F " << position[FRONT]
                    << "FL " << position[FRONT_LEFT]
                    << "FR " << position[FRONT_RIGHT]
                    << "R " << position[REAR]
                    << "RL " << position[REAR_LEFT]
                    << "RR " << position[REAR_RIGHT];*/
    }else if(s->getName()=="analog sharps"){
        QList<double> v=s->getValues();
        position[RIGHT]=v[0];
        position[REAR]=v[1];
        position[FRONT]=v[2];
        position[LEFT]=v[3];
    }
    for(double &p : position){
        if(p<0.1){
            p=0;
        }
    }
}

void Robot::checkFloor(Sensor *s)
{
    if(!s->getValues()[0]==1){
        qDebug()<<"Nie ma podłogi :(";
        //state=ZAWRACANIE;
    }else{

        qDebug()<<"Podłoga :) "<<s->getValues()[0];
    }
}

State Robot::getState() const
{
    return state;
}

void Robot::setState(const State &value)
{
    if(state=value){
        return;
    }
    state = value;
    if(value==NA_WPROST){
        velocity=Velocity(0.5,0);
    }
    if(value==W_LEWO){
        velocity=Velocity(0.5,0.6);
    }
    if(value==W_PRAWO){
        velocity=Velocity(0.5,-0.6);
    }
    if(value==STOP){
        velocity=Velocity(0.0,0.0);
    }
    motorLeft->setSP(velocity.getLeftSp());
    motorRight->setSP(velocity.getRightSp());
}

bool Robot::checkObstacle(Direction dir, double max, double min)
{
    if(dir<EXT_FRONT && position[dir]<max && position[dir]>min){
        return true;
    }else if(dir==EXT_FRONT && (checkObstacle(FRONT)||checkObstacle(FRONT_LEFT)||checkObstacle(FRONT_RIGHT))){
        return true;
    }else if(dir==EXT_BACK && (checkObstacle(REAR)||checkObstacle(REAR_LEFT)||checkObstacle(REAR_RIGHT))){
        return true;
    }
    return false;
}

void Robot::checkState()
{
    if(state==NA_WPROST){
        if(position[FRONT]<15 && position[FRONT]>0
                && position[FRONT_LEFT]<20 && position[FRONT_LEFT]>0
                && position[FRONT_RIGHT]<20 && position[FRONT_RIGHT]){
            if(position[FRONT_LEFT]<20 && position[FRONT_LEFT]>0
                    && position[LEFT]<20 && position[LEFT]>0){
                if(position[FRONT_RIGHT]<20 && position[FRONT_RIGHT]
                        && position[RIGHT]<20 && position[RIGHT]>0){
                    setState(STOP);
                }else{
                    setState(W_PRAWO);
                }
            }else{
                setState(W_LEWO);
            }
        }
    }else if(state=W_LEWO){
        if((!checkObstacle(FRONT)) && (!checkObstacle(RIGHT))){
            setState(NA_WPROST);
        }
    }else if(state=W_PRAWO){
        if((!checkObstacle(FRONT)) && (!checkObstacle(LEFT))){
            setState(NA_WPROST);
        }
    }else if(state=STOP){
        ;//stój na wieki
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
    //qDebug() << "still alive";
    //QProcess::execute("clear");
    system("cls");
    qDebug() << "F " << position[FRONT]
                        << "FL " << position[FRONT_LEFT]
                        << "LEWO " << position[LEFT]
                        << "FR " << position[FRONT_RIGHT]
                        << "PRAWO " << position[RIGHT]
                        << "R " << position[REAR]
                        << "RL " << position[REAR_LEFT]
                        << "RR " << position[REAR_RIGHT];
    qDebug()<<"stan: "<< state;
    qDebug()<<"rand"<<qrand();
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

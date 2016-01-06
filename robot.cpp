#include "robot.h"
#include "tcpserver.h"

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
    //database.setHostName("localhost");
    database.setPort(3306);
    database.setDatabaseName("herring");
    //database.setDatabaseName("test1");
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
        qWarning() << "Nawiązano połączenie z bazą danych.";
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
    server = new TcpServer(this);
    server->setRobot(this);
    if(!server->listen(QHostAddress::Any, 7557)){
        qWarning() << "Nie udało się uruchomić serwera :(";
    }

    qWarning() << server->serverAddress().toString();
    qWarning() << server->serverPort();
    /*
     * Inicjalizacja peryferiów
     */
    qDebug() << "Inicjalizacja sensorów...";
    sonar = new Sonar("sonars");
    sharp = new Sharp("analog sharps");
    frontFloor = new Floor("floor front");
    rearFloor = new Floor("floor back");
    battery = new Battery("battery");
    system = new Peryferium("system");
    auto delegatUpdatePosition=[&](Peryferium* p)mutable{
        Sensor* s=dynamic_cast<Sensor*>(p);
        if(s){
            updatePosition(s);
        }
    };
    auto delegateCheckFloor=[&](Peryferium* p)mutable{
        Sensor* s=dynamic_cast<Sensor*>(p);
        if(s){
            checkFloor(s);
        }
    };
    auto delegateCheckVoltage=[&](Peryferium* p)mutable{
        Sensor* s=dynamic_cast<Sensor*>(p);
        if(s){
            checkVoltage(s);
        }
    };
    auto delegateSystemReset=[&](Peryferium* p)mutable{
        systemReset();
    };
    system->setCallbackFunction(delegateSystemReset);
    sharp->setCallbackFunction(delegatUpdatePosition);
    sonar->setCallbackFunction(delegatUpdatePosition);
    frontFloor->setCallbackFunction(delegateCheckFloor);
    rearFloor->setCallbackFunction(delegateCheckFloor);
    battery->setCallbackFunction(delegateCheckVoltage);
    sonar->autoMeasure(1000);
    sharp->autoMeasure(1000);
    frontFloor->autoMeasure(1000);
    rearFloor->autoMeasure(1000);
    battery->autoMeasure(1000);
    qDebug() << "Sensory OK.";
    motorLeft = new Motor("motor left");
    motorRight = new Motor("motor right");
    setupMotors();
    /*
     * misc, testowe
     */
    //sonar = new Sensor("sonary");
    //position = QVector<double>(8);
    timer=new QTimer(this);
    timer->setInterval(500);
    timer->start();
    connect(timer,SIGNAL(timeout()),SLOT(timerHandler()));

    velocity=Velocity(0,0);
    control=HAND;
    setState(ROZRUCH);
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
        if(v.size()<5){
            return;
        }
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
        if(v.size()<4){
            return;
        }
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
    checkState();
}

void Robot::checkFloor(Sensor *s)
{
    if(s->getValues().isEmpty()){
        return;
    }
    if(!s->getValues()[0]==1){
        qDebug()<<"Nie ma podłogi :(";
        //state=ZAWRACANIE;
    }else{

        qDebug()<<"Podłoga :) "<<s->getValues()[0];
    }
}

void Robot::checkVoltage(Sensor *s)
{
    if(s->getValues().size()<1){
        return;
    }
    voltage.enqueue(s->getValues()[0]);
    qDebug()<<"Napiecie "<<voltage;
    int n=15;
    if(voltage.size()> n){
        voltage.dequeue();
        double meanV=0;
        for(int i=0;i<n;i++){
            meanV+=voltage[i];
        }
        meanV/=n;
        QList<double> validV;
        for(int i=0;i<n;i++){
            if(voltage[i]<meanV*1.5 && voltage[i]>meanV*0.5){
                validV.append(voltage[i]);
            }
        }
        meanV=0;
        n=validV.size();
        for(int i=0;i<n;i++){
            meanV+=validV[i];
        }
        meanV/=n;
        qDebug()<<"Napiecie "<<meanV;
        if(meanV<6.3){
            qDebug() << "Napięcie na niskie!";
            ::system("shutdown -s");
        }
    }
}

State Robot::getState() const
{
    return state;
}

void Robot::setState(const State &value)
{
    qDebug() << "Set state";
    qWarning() << "STAN: " << state;
    if(value==AUTO_ON){
        control=AUTO;
        setState(ROZRUCH);
        checkState();
    }
    if(state==value){
        return;
    }
    bool toggleDir=false;
    if(value==TYL || state==TYL){
        toggleDir=true;
    }
    state = value;
    qWarning() << "ZMIANA STANU " << state;
    if(value==NA_WPROST){
        velocity=Velocity(0.7,0);
    }
    if(value==W_LEWO){
        velocity=Velocity(0.7,0.5);
    }
    if(value==W_PRAWO){
        velocity=Velocity(0.7,-0.5);
    }
    if(value==STOP){
        velocity=Velocity(0.0,0.0);
    }
    if(value==TYL){
        velocity=Velocity(-0.7,0.0);
    }
    if(toggleDir){
        motorLeft->toggleDir();
        motorRight->toggleDir();
    }
    motorLeft->setSP(velocity.getLeftSp());
    motorRight->setSP(velocity.getRightSp());
    //motorLeft->setCV(velocity.getLeftSp());
    //motorRight->setCV(velocity.getRightSp());
}

Control Robot::getControl() const
{
    return control;
}

void Robot::setControl(const Control &value)
{
    control = value;
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

void Robot::setupMotors()
{
    motorLeft->setKp(20);
    //motorLeft->setTi(0.5);
    //motorLeft->setTd(120);
    motorLeft->setDir(1);
    motorRight->setKp(20);
    //motorRight->setTi(0.5);
    //motorRight->setTd(120);
    motorRight->setDir(1);
    //motorLeft->setPid(0);
}

void Robot::checkState()
{
    if(control==HAND){
        return;
    }
    if(state==ROZRUCH){
        setState(NA_WPROST);
    }
    if(state==NA_WPROST){
        /*if(position[FRONT]<15 && position[FRONT]>0
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
        }*/
        if(checkObstacle(FRONT)){
            if(checkObstacle(LEFT)){
                if(checkObstacle(RIGHT)){
                    setState(STOP);
                }else{
                    setState(W_PRAWO);
                }
            }else{
                if(checkObstacle(RIGHT)){
                    setState(W_LEWO);
                }else{
                    setState((qrand()%2==1)?W_LEWO:W_PRAWO);
                }
            }
        }else{
            setState(NA_WPROST);
        }
    }else if(state==W_LEWO){
        if((!checkObstacle(FRONT)) && (!checkObstacle(FRONT_RIGHT))){// && (!checkObstacle(RIGHT))){
            setState(NA_WPROST);
        }else if(checkObstacle(FRONT) && checkObstacle(LEFT) && checkObstacle(RIGHT)){
            setState(STOP);
        }
    }else if(state==W_PRAWO){
        if((!checkObstacle(FRONT)) && (!checkObstacle(FRONT_LEFT))){
            setState(NA_WPROST);
        }else if(checkObstacle(EXT_FRONT) && checkObstacle(LEFT) && checkObstacle(RIGHT)){
            setState(STOP);
        }
    }else if(state==STOP){
        if(!checkObstacle(EXT_FRONT)){
            setState(NA_WPROST);
        }
    }
}

void Robot::systemReset()
{
    port->resetQueue();
    setupMotors();
    setState(ROZRUCH);
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
    //system("cls");
    qDebug() << "F " << position[FRONT]
                        << "FL " << position[FRONT_LEFT]
                        << "LEWO " << position[LEFT]
                        << "FR " << position[FRONT_RIGHT]
                        << "PRAWO " << position[RIGHT]
                        << "R " << position[REAR]
                        << "RL " << position[REAR_LEFT]
                        << "RR " << position[REAR_RIGHT];

    QVector<bool> obstacles(10);
    for(int i=0;i<obstacles.size();i++){
        obstacles[i]=checkObstacle(Direction(i));
    }
    qDebug()<<"FRONT"<<obstacles[FRONT]
              <<"FRONT_LEFT"<<obstacles[FRONT_LEFT]
                <<"FRONT_RIGHT"<<obstacles[FRONT_RIGHT]
                  <<"LEFT"<<obstacles[LEFT]
                    <<"RIGHT"<<obstacles[RIGHT]
                      <<"REAR"<<obstacles[REAR]
                        <<"REAR_LEFT"<<obstacles[REAR_LEFT]
                          <<"REAR_RIGHT"<<obstacles[REAR_RIGHT]
                            <<"EXT_FRONT"<<obstacles[EXT_FRONT]
                              <<"EXT_BACK"<<obstacles[EXT_BACK];
    qDebug()<<"stan: "<< state;
    qDebug()<<"rand"<<qrand();
}

QSqlDatabase* Robot::db=nullptr;

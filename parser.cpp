#include "parser.h"
#include <QTime>
#include "sensor.h"
#include "actuator.h"
#include "comport.h"

Parser* Parser::parser=0;

Parser::Parser(QSerialPort *_port)
{
    pointer=0;
    state=WAITING;
    port=_port;
    parser=this;
    qDebug() << port;
    qDebug() << port->portName();
    //connect(port,SIGNAL(readyRead()),this,SLOT(dataAvilable()));
    connect(port, SIGNAL(readyRead()), SLOT(dataAvilable()));
}

void Parser::parse(QString message)
{
    //qDebug() << "parse";
    //qDebug() << message;
    QList<QString> words;//=message.split("-");
    int splitPoint=0;
    for(int i=0;i<message.size();i++){
        if(message[i]=='-'){
            words.append(message.mid(splitPoint,i-splitPoint));
            splitPoint=i+1;
            if(words.size()==2){
                break;
            }
        }
    }
    if(splitPoint<message.size()){
        words.append(message.mid(splitPoint,message.size()-splitPoint));
    }
    qDebug()<<words;
    if(words.size()<2){
        return;//exception?
    }
    Peryferium* p=findByName(words[0]);
    if(!p){
        return;
    }
    Sensor* s=dynamic_cast<Sensor*>(p);
    if(s && words[1]=="measurement"){
        if(words.size()>=3){
            QList<double> values=stringToDoubleList(words[2]);
            s->setValues( values );
        }
        if(s->callbackFunction){
            //s->callbackFunction(s);
        }
    }
    Actuator* a=dynamic_cast<Actuator*>(p);
    if(!s && !a){
        if(p->getCallbackFunction()){
            p->getCallbackFunction()(p);
        }
    }
}

QByteArray Parser::getBuffer()
{
    return buffer;
}

bool Parser::registerPeryferium(Peryferium *p)
{
    if(parser){
        parser->addPeryferium(p);
        return true;
    }else{
        return false;
    }
}

bool Parser::unregisterPeryferium(Peryferium *p)
{
    if(parser){
        return parser->removePeryferium(p);
    }else{
        return false;
    }
}

void Parser::addPeryferium(Peryferium *p)
{
    parser->peryferia.append(p);
}

bool Parser::removePeryferium(Peryferium *p)
{
    return peryferia.removeAll(p);
}

Peryferium *Parser::findByName(QString name)
{
    for(int i=0; i<peryferia.size();i++){
        if(peryferia[i]->getName()==name){
            return peryferia[i];
        }
    }
    return 0;
}

QList<double> Parser::stringToDoubleList(QString str)
{
    QList<double> result;
    QList<QString> words=str.split(",",QString::SkipEmptyParts);
    double r=0;
    bool ok=false;
    for(int i=0; i<words.size(); i++){
        r=words[i].toDouble(&ok);
        if(ok){
            result.append(r);
        }else{
            result.append(0);
        }
    }
    return result;
}

QString Parser::doubleListToString(QList<double> values)
{
    QString result;
    for(double v : values){
        result+=QString::number(v)+",";
    }
    if(result.size()>1){
        result=result.mid(0,result.size()-1);
    }
    return result;
}

void Parser::dataAvilable()
{
    if(port->bytesAvailable()){
        buffer.append(port->readAll());
        qDebug() << "bytes avilable";
        qDebug() << buffer;
        process();
    }
    /*qDebug() << "data avilable";
    qDebug() << port->bytesAvailable();
    QByteArray tmpData=port->readAll();
    qDebug() << tmpData[0];
    qDebug() << tmpData.data();
    parse(QString(tmpData));*/
}

void Parser::process()
{
    while(pointer<buffer.size()){
        //qDebug() << buffer[pointer];
        //qDebug() << state;
        if(buffer[pointer]==':'){
            buffer=buffer.mid(pointer+1);
            pointer=-1;
            state=IN_PROGRESS;
        }else if(buffer[pointer]==';'){
            if(state==IN_PROGRESS){
                QByteArray msg=buffer.left(pointer);
                state=WAITING;
                qDebug() << msg;
                qInfo() << "->" << msg;
                qInfo() << QTime::currentTime().toString("hh:mm:ss,zzz");
                emit commandReceived();
                (static_cast<ComPort*>(port))->goOn();
                parse(QString(msg));
            }
            buffer=buffer.mid(pointer+1);
            pointer=-1;
        }
        pointer++;
    }
}
QSerialPort *Parser::getPort() const
{
    return port;
}

void Parser::setPort(QSerialPort *value)
{
    port = value;
}


#include "command.h"

Command::Command()
{
    strCommand=":";
}

Command::~Command()
{

}

QString Command::string()
{
    return strCommand;
}

Command Command::operator<<(QString str)
{
    if(strCommand.size()>1){
        strCommand+="-";
    }
    strCommand+=str;
    return *this;
}

Command Command::operator<<(double d)
{
    if(strCommand.size()>1){
        strCommand+="-";
    }
    strCommand+=QString::number(d);
    return *this;
}

Command Command::operator<<(QList<double> dl)
{
    if(strCommand.size()>1){
        strCommand+="-";
    }
    strCommand+=Parser::doubleListToString(dl);
    return *this;
}

Command Command::operator<<(Command cmd)
{
    if(strCommand.size()>1){
        strCommand+="-";
    }
    strCommand+=cmd.string().mid(1);
    return *this;
}

QByteArray Command::byteArray()
{
    QString tmpCmd=strCommand;
    tmpCmd+=";";
    return QByteArray(tmpCmd.toStdString().c_str());
}


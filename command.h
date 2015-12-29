#ifndef COMMAND_H
#define COMMAND_H

#include <QString>
#include <QByteArray>
#include "parser.h"

class Command
{
public:
    Command();
    ~Command();
    Command operator<<(QString str);
    Command operator<<(double d);
    Command operator<<(QList<double> dl);
    Command operator<<(Command cmd);
    QByteArray byteArray();
    QString string();
private:
    QString strCommand;
};

#endif // COMMAND_H

#include "battery.h"
#include "robot.h"

Battery::Battery(QString _name)
    :Sensor(_name)
{

}

void Battery::updateDB()
{
    if(values.size()<1){
        qDebug() << "Brak danych (Battery).";
        return;
    }
    QString statement="UPDATE `herring`.`batery` SET `voltage`=';";
    statement+=QString::number(values[0]);
    statement+="' WHERE `idbatery`='1';";
    Robot::db->exec(statement);
}


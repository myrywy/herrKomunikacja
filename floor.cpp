#include "floor.h"
#include "robot.h"

Floor::Floor(QString _name)
    :Sensor(_name)
{

}

void Floor::updateDB()
{
    if(values.size()<1){
        qDebug() << "Brak danych (Sonar).";
        return;
    }
    for(int i=0; i<values.size(); i++){
        QString statement="UPDATE `herring`.`floor` SET `state`='";
        statement+=QString::number(values[i]);
        statement+="' WHERE `idfloor`='";
        statement+=QString::number(i+1);
        statement+="';";
        Robot::db->exec(statement);
    }
}


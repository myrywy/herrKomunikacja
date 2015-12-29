#include "sonar.h"
#include "robot.h"

Sonar::Sonar(QString _name)
    :Sensor(_name)
{

}

void Sonar::updateDB()
{
    if(values.size()<1){
        qDebug() << "Brak danych (Sonar).";
        return;
    }
    for(int i=0; i<values.size(); i++){
        QString statement="UPDATE `herring`.`sonar` SET `distance`='";
        statement+=QString::number(values[i]);
        statement+="' WHERE `sonarId`='";
        statement+=QString::number(i+1);
        statement+="';";
        Robot::db->exec(statement);
    }
}


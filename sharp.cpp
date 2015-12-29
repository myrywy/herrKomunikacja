#include "sharp.h"
#include "robot.h"

Sharp::Sharp(QString _name)
    :Sensor(_name)
{

}

void Sharp::updateDB()
{
    if(values.size()<1){
        qDebug() << "Brak danych (Sonar).";
        return;
    }
    for(int i=0; i<values.size(); i++){
        QString statement="UPDATE `herring`.`sharp` SET `distance`='";
        statement+=QString::number(values[i]);
        statement+="' WHERE `idsharp`='";
        statement+=QString::number(i+1);
        statement+="';";
        Robot::db->exec(statement);
    }
}


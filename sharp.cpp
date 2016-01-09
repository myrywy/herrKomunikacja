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
    QVector<double> tmpMeas(4);
    tmpMeas[0]=values[2];
    tmpMeas[1]=values[1];
    tmpMeas[2]=values[3];
    tmpMeas[3]=values[0];
    for(int i=0; i<values.size(); i++){
        QString statement="UPDATE `herring`.`sonar` SET `distance`='";
        statement+=QString::number(tmpMeas[i]);
        statement+="' WHERE `sonarId`='";
        statement+=QString::number(i+7);
        statement+="';";
        Robot::db->exec(statement);
    }
}


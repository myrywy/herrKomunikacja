#include "velocity.h"
#include <QDebug>

Velocity::Velocity()
{

}

Velocity::Velocity(double _linear, double _angular)
    :linear(_linear),
      angular(_angular)
{

}

double Velocity::getLeftSp()
{
    qDebug() << linear << "," << angular << " - leftSP = " << maxSP*linear*(1-angular);
    return maxSP*linear*(1-angular);
}

double Velocity::getRightSp()
{
    return maxSP*linear*(1+angular);
}

double Velocity::maxSP=2.5;

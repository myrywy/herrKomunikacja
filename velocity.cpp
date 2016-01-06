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
    //qDebug() << linear << "," << angular << " - leftSP = " << maxSP*linear*(1-angular);
    return qAbs(maxSP*linear)*(1-angular);
}

double Velocity::getRightSp()
{
    return qAbs(maxSP*linear)*(1+angular);
}

double Velocity::getLeftCv()
{
    return 5000-maxCV*linear*(1-0.5*angular);
}

double Velocity::getRightCv()
{
    return 5000-maxCV*linear*(1+0.5*angular);
}

double Velocity::maxSP=1;
double Velocity::maxCV=2500;
bool Velocity::openSystem=false;

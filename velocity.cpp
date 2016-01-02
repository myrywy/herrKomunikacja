#include "velocity.h"

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
    return maxSP*linear*(1-angular);
}

double Velocity::getRightSp()
{
    return maxSP*linear*(1+angular);
}

double Velocity::maxSP=2.5;

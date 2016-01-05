#ifndef VELOCITY_H
#define VELOCITY_H


class Velocity
{
public:
    Velocity();
    Velocity(double _linear, double _angular);
    /*
     * Liniowa składowa prędkości w jednostkach umownych od 0 do 1;
     */
    double linear;
    /*
     * Kątowa składowa prędkości w jednostkach umownych od 0 do 1;
     */
    double angular;
    double getLeftSp();
    double getRightSp();
    double getLeftCv();
    double getRightCv();
    static double maxSP;
    static double maxCV;
    static bool openSystem;
};

#endif // VELOCITY_H

#ifndef CAMERA_H
#define CAMERA_H

#include <QObject>
#include "servo.h"
#include "enums.h"

class Camera : public QObject
{
    Q_OBJECT
public:
    explicit Camera(QObject *parent = 0);
    void setUpperAngle(int angle);
    void setLowerAngle(int angle);
    void setUpperVelocity(double v);
    void setLowerVelocity(double v);
    void setMove(CameraMove movement);
private:
    Servo* upper;
    Servo* lower;
    const double defaultVelocity;
signals:

public slots:
};

#endif // CAMERA_H

#include "camera.h"

Camera::Camera(QObject *parent) : QObject(parent),
    defaultVelocity(20)
{
    lower=new Servo("serwo lower");
    upper=new Servo("serwo upper");
    lower->setParent(this);
    upper->setParent(this);
}

void Camera::setUpperAngle(int angle)
{
    upper->setAngle(angle);
}

void Camera::setLowerAngle(int angle)
{
    lower->setAngle(angle-90);
}

void Camera::setUpperVelocity(double v)
{
    upper->setVelocity(v);
}

void Camera::setLowerVelocity(double v)
{
    lower->setVelocity(v);
}

void Camera::setMove(CameraMove movement)
{
    switch(movement){
    case CameraMove::UP:
        upper->setVelocity(defaultVelocity);
        break;
    case CameraMove::DOWN:
        upper->setVelocity(-defaultVelocity);
        break;
    case CameraMove::LEFT:
        lower->setVelocity(-defaultVelocity);
        break;
    case CameraMove::RIGHT:
        lower->setVelocity(defaultVelocity);
        break;
    default:
        lower->setVelocity(0);
        upper->setVelocity(0);
    }
}

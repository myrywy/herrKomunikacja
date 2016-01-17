#include "detectionprocess.h"
#include <opencv2/highgui/highgui.hpp>
DetectionProcess::DetectionProcess(int _index, QObject *parent)
    : QObject(parent),
      index(_index),
      lock(false)
{

}

cv::Mat DetectionProcess::getImg() const
{
    return img;
}

bool DetectionProcess::setImg(const cv::Mat &value)
{
    if(value.empty() && !locked()){
        return false;
    }
    img = value;
    emit imgLoaded();
    return true;
}

bool DetectionProcess::locked()
{
    return lock;
}

void DetectionProcess::processImage()
{
    if(lock || img.empty()){
        return;
    }
    lock=true;
    cv::pyrDown(img,img);
    cv::pyrDown(img,img);
    imshow(QString::number(index).toStdString().c_str(),img);
    lock=false;
    emit finished();
}

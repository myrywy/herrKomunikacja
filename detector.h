#ifndef DETECTOR_H
#define DETECTOR_H

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/ml/ml.hpp>

class Detector
{
public:
    Detector();
    virtual bool predict(cv::Mat img)=0;
    virtual bool loadFromFile(QString filename)=0;
};
#endif // DETECTOR_H

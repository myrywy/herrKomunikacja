#ifndef CLASSIFIER_H
#define CLASSIFIER_H

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/ml/ml.hpp>
#include <QString>

class Classifier
{
public:
    Classifier();
    virtual bool predict(cv::Mat img)=0;
    virtual bool loadFromFile(QString filename)=0;
};
#endif // CLASSIFIER_H

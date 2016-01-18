#ifndef EXHAUSTIVESEARCH_HPP
#define EXHAUSTIVESEARCH_HPP

#include <cstdlib>
#include <functional>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <QVector>
#include "functionalutils.hpp"

using namespace cv;

class ExhaustiveSearch
{
public:
    ExhaustiveSearch();

    double getStride() const;
    void setStride(double value);

    double getRatio() const;
    void setRatio(double value);

    int getWidth() const;
    void setWidth(int value);

    std::function<bool (Mat &)> getDetectionFunction() const;
    void setDetectionFunction(const std::function<bool (Mat &)> &value);

    QVector<cv::Rect> operator()(Mat& img);

    QVector<double> getScales() const;
    void setScales(const QVector<double> &value);

private:
    double stride;
    double ratio;
    int width;
    QVector<double> scales;
    std::function<bool(Mat&)> detectionFunction;
};

#endif // EXHAUSTIVESEARCH_HPP

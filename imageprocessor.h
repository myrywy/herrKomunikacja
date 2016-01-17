#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include <QTimer>
#include <QThread>
#include <QVector>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "detectionprocess.h"
#include "task.h"

class ImageProcessor : public QObject
{
    Q_OBJECT
public:
    explicit ImageProcessor(int streamIndex, unsigned interval=30, int _threadsNumber=4, QObject *parent = 0);

private:
    QTimer* timer;
    cv::VideoCapture stream;
    QVector<Task*> threads;
    QVector<DetectionProcess*> detectors;
    const int threadsNumber;
    void tasksInit();
    int iter;
signals:
    void frameCaptured(cv::Mat frame,int i);
public slots:
    void nextFrame();
};

#endif // IMAGEPROCESSOR_H

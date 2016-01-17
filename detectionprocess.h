#ifndef DETECTIONPROCESS_H
#define DETECTIONPROCESS_H

#include <QObject>
#include <opencv2/imgproc/imgproc.hpp>

class DetectionProcess : public QObject
{
    Q_OBJECT
public:
    explicit DetectionProcess(int _index, QObject *parent = 0);
    cv::Mat getImg() const;
    bool setImg(const cv::Mat &value);
    bool locked();
private:
    int index;
    bool lock;
    cv::Mat img;
signals:
    void finished();
    void imgLoaded();
public slots:
    void processImage();
};

#endif // DETECTIONPROCESS_H

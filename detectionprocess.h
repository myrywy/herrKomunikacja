#ifndef DETECTIONPROCESS_H
#define DETECTIONPROCESS_H

#include <QObject>
#include <QSqlDatabase>
#include <opencv2/imgproc/imgproc.hpp>
#include "exhaustivesearch.hpp"
#include "classifier.h"
#include <QImage>

class DetectionProcess : public QObject
{
    Q_OBJECT
public:
    explicit DetectionProcess(int _index, QObject *parent = 0);
    virtual ~DetectionProcess();
    cv::Mat getImg() const;
    bool setImg(const cv::Mat &value);
    bool locked();
    void setDb(QSqlDatabase *value);
    static QImage MatToQImage(cv::Mat m);
private:
    int index;
    bool lock;
    cv::Mat img;
    ExhaustiveSearch search;
    Classifier* classifier;
    QSqlDatabase* db;
    void reportDetection();
    void updateDB();
signals:
    void finished();
    void imgLoaded();
public slots:
    void processImage();
};

#endif // DETECTIONPROCESS_H

#include "detectionprocess.h"
#include <opencv2/highgui/highgui.hpp>
#include <QPixmap>
#include <QSqlQuery>
#include <QByteArray>
#include <QBuffer>
#include <QFile>

DetectionProcess::DetectionProcess(int _index, QObject *parent)
    : QObject(parent),
      index(_index),
      lock(false),
      classifier(nullptr),
      db(nullptr)
{
    search.setStride(0.2);
    search.setScales(QVector<double>{1,0.75,0.5});
    search.setWidth(160);
    //classifier =new
    if(classifier){
        std::function<bool(cv::Mat)> dFunction([&](cv::Mat m)->bool{
            return classifier->predict(m);
        });
        search.setDetectionFunction(dFunction);
    }
}

DetectionProcess::~DetectionProcess()
{
}

cv::Mat DetectionProcess::getImg() const
{
    return img;
}

bool DetectionProcess::setImg(const cv::Mat &value)
{
    if(value.empty() || locked()){
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

void DetectionProcess::setDb(QSqlDatabase *value)
{
    db = value;
}

QImage DetectionProcess::MatToQImage(Mat m)
{
     cv::Mat tmp;
     cvtColor(m, tmp,CV_BGR2RGB);
     QImage dest((const uchar *) tmp.data, tmp.cols, tmp.rows, tmp.step, QImage::Format_RGB888);
     dest.bits();
     return dest;
}

void DetectionProcess::reportDetection()
{

}

void DetectionProcess::updateDB()
{
    if(db && index==0){
        //QImage qimg=MatToQImage(img);
        //QPixmap pixmap=QPixmap::fromImage(qimg,Qt::MonoOnly);
        /*pixmap.loadFromData(img.data,img.cols*img.rows*3,"JPG");

        QByteArray imgByteArray;
        QBuffer imgBuffer(&imgByteArray);
        imgBuffer.open(QIODevice::WriteOnly);
        pixmap.save(&imgBuffer,"JPG");
        int pixN=img.cols*img.rows*3;
        QByteArray imgByteArray((char*)(img.data),pixN);*/
        QString filename=QString::number(index)+QString(".PNG");
        cv::imwrite(
                    (filename).toStdString().c_str(),
                    img);
        /*QImage qImg(filename,"PNG");
        //QPixmap pixmap;
        //pixmap.load(filename,"JPG");
        QByteArray imgByteArray;
        QBuffer imgBuffer(&imgByteArray);
        imgBuffer.open(QIODevice::WriteOnly);
        qImg.save(&imgBuffer,"PNG");*/
        QFile file(filename);
        if(!file.open(QIODevice::ReadOnly)){
            return;
        }
        QByteArray imgByteArray=file.readAll();
        QSqlQuery query = QSqlQuery(*db);
        query.prepare("UPDATE `herring`.`image` SET `image`=:imgdata WHERE `imageId`='1';");
        query.bindValue(":imgdata", imgByteArray);
        system("clc");
        qWarning() << "Image DB update: " << query.exec();
    }
    system("clc");
    qWarning() << "No DB to update :() ";
}

void DetectionProcess::processImage()
{
    if(lock || img.empty()){
        return;
    }
    lock=true;
    cv::pyrDown(img,img);
    //cv::pyrDown(img,img);

    updateDB();

    imshow(QString::number(index).toStdString().c_str(),img);
    QVector<cv::Rect> results;
    results=search(img);

    for(int i=0;i<results.size();i++){
        Rect r=results[i];
        rectangle(img, r.tl(), r.br(), cv::Scalar(0,255,0), 2);
    }

    if(results.size()!=0){
        reportDetection();
    }

    lock=false;
    emit finished();
}

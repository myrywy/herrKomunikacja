#include "imageprocessor.h"

ImageProcessor::ImageProcessor(int streamIndex, unsigned interval, int _threadsNumber, QObject *parent)
    : QObject(parent),
      stream(streamIndex),
      threadsNumber(_threadsNumber),
      iter(0),
      db(nullptr)
{
    tasksInit();
    timer=new QTimer(this);
    timer->setInterval(interval);
    connect(timer,SIGNAL(timeout()),SLOT(nextFrame()));
    timer->start();
}

void ImageProcessor::tasksInit()
{
    threads=QVector<Task*>(threadsNumber);
    detectors=QVector<DetectionProcess*>(threadsNumber);
    for(int i = 0; i<threadsNumber; i++){
        threads[i]=new Task();
        detectors[i]=new DetectionProcess(i);
        detectors[i]->moveToThread(threads[i]);
        connect(threads[i],SIGNAL(started()),detectors[i],SLOT(processImage()));
        connect(this,SIGNAL(destroyed(QObject*)),threads[i],SLOT(deleteLater()));
        connect(this,SIGNAL(destroyed(QObject*)),detectors[i],SLOT(deleteLater()));
        //connect(detectors[i],SIGNAL(finished()),threads[i],SLOT(quit()));
        //connect(detectors[i],SIGNAL(finished()),threads[i],SLOT(terminate()));
        connect(detectors[i],SIGNAL(imgLoaded()),detectors[i],SLOT(processImage()));
        if(db){
            detectors[i]->setDb(db);
        }
        threads[i]->start();
    }
}

void ImageProcessor::setDb(QSqlDatabase *value)
{
    db = value;
    if(db){
        for(DetectionProcess* &d : detectors){
            d->setDb(db);
        }
    }
}

void ImageProcessor::nextFrame()
{
    cv::Mat tmp;
    stream >> tmp;
    if(detectors[iter]->setImg(tmp)){
        //threads[iter]->start();
    }
    ++iter;
    if(iter>=threadsNumber){
        iter=0;
    }
}

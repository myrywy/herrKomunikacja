#include "exhaustivesearch.hpp"
#include "QtAlgorithms"
ExhaustiveSearch::ExhaustiveSearch()
{
}
double ExhaustiveSearch::getStride() const
{
    return stride;
}

void ExhaustiveSearch::setStride(double value)
{
    stride = value;
}
double ExhaustiveSearch::getRatio() const
{
    return ratio;
}

void ExhaustiveSearch::setRatio(double value)
{
    ratio = value;
}
int ExhaustiveSearch::getWidth() const
{
    return width;
}

void ExhaustiveSearch::setWidth(int value)
{
    width = value;
}
std::function<bool (Mat &)> ExhaustiveSearch::getDetectionFunction() const
{
    return detectionFunction;
}

void ExhaustiveSearch::setDetectionFunction(const std::function<bool(Mat &)> &value)
{
    detectionFunction = value;
}

QVector<Rect> ExhaustiveSearch::operator()(Mat &img)
{
    if(!detectionFunction){
        return QVector<Rect>{};
    }
    QList<Rect> rois;
    int height=width/ratio;
    for(int i=0; i<scales.size(); i++){
        //qDebug()<< "SKALA ------ " << scales[i];
        int x=0;
        while(int(x+scales[i]*width)<img.cols){
            int y=0;
            while(int(y+scales[i]*height)<img.rows){
                //qDebug() << "ex()out " << x << y << width*scales[i] << height*scales[i];
                rois.append(Rect(x,y,width*scales[i],height*scales[i]));
                y+=int(height*stride*scales[i]);
            }
            x+=int(width*stride*scales[i]);
        }
    }
    std::function<bool(Rect)> f=[this, img](Rect r)->bool{
        //qDebug() << "ex() " << r.x << r.y << r.width << r.height;
        Mat roi=Mat(img,r);
        //qDebug() << "roi done";
        return detectionFunction(roi);
    };
    QList<bool> probs=mapC(rois,f);
    QVector<Rect> results;
    for(int i=0; i<rois.size();i++){
        if(probs[i]){
            results.append(rois[i]);
        }
    }
    return results;
}
QVector<double> ExhaustiveSearch::getScales() const
{
    return scales;
}

void ExhaustiveSearch::setScales(const QVector<double> &value)
{
    scales = value;
}






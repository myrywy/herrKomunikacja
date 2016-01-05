#ifndef COMPORT_H
#define COMPORT_H


#include <QtSerialPort/QSerialPort>
#include <QList>
#include <QQueue>
#include <QTimer>

class ComPort : public QSerialPort
{
    Q_OBJECT
public:
    //ComPort();
    ComPort(QString _portName);
    virtual qint64 writeData(const char * data, qint64 maxSize);
    void resetQueue();
protected:
    int maxWaitTime;
    QQueue<QByteArray> queue;
    bool waiting;
    void wait(bool _wait=true);
    QTimer* timer;
public slots:
    void goOn();
    void waitingTimeout();
};

#endif // COMPORT_H

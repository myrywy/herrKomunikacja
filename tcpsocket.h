#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <QTcpSocket>

class TcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    TcpSocket();
public slots:
    void readyReadHandler();
signals:
    void dataReady(TcpSocket* s);
};

#endif // TCPSOCKET_H

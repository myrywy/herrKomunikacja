#ifndef RETRANSMITER_H
#define RETRANSMITER_H

#include <QObject>
#include <QTcpSocket>

class Retransmiter : public QObject
{
    Q_OBJECT
public:
    Retransmiter(QTcpSocket *_socket);

    QTcpSocket* socket;
public slots:
    void reemit();
signals:
    void readyRead(QTcpSocket*);
};

#endif // RETRANSMITER_H

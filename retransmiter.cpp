#include "retransmiter.h"

Retransmiter::Retransmiter(QTcpSocket *_socket){
    socket=_socket;
    connect(socket,SIGNAL(destroyed(QObject*)),this,SLOT(deleteLater()));
}

void Retransmiter::reemit(){
    emit readyRead(socket);
}

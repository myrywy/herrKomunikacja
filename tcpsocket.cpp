#include "tcpsocket.h"

TcpSocket::TcpSocket()
{

}

void TcpSocket::readyReadHandler()
{
    emit dataReady(this);
}

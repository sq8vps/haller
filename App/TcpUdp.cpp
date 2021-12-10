#include "TcpUdp.h"
#include <QDebug>
#include <QHostAddress>

#define BASIC_STR(x) #x
#define STR(x) BASIC_STR(x)

TcpUdp::TcpUdp(QObject *parent) : QObject(parent)
{
    udpSocket = new QUdpSocket(this);
    tcpServer = new QTcpServer(this);
}

QString TcpUdp::tcp_listen()
{
   connect(tcpServer, SIGNAL(newConnection()), this, SLOT(tcp_onNewConnection()));
   if(tcpServer->listen(QHostAddress(LOCAL_ADDRESS), TCP_PORT_LOCAL))
       return "OK";

   return "FAIL";
}

void TcpUdp::tcp_onNewConnection(void)
{
    clientSocket = tcpServer->nextPendingConnection();
    connect(clientSocket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(tcp_onStateChanged(QAbstractSocket::SocketState)));
    connect(clientSocket, SIGNAL(readyRead()), this, SLOT(tcp_onReadyRead()));
    emit onClientConnected();
}

void TcpUdp::tcp_onStateChanged(QAbstractSocket::SocketState socketState)
{
    if ((socketState == QAbstractSocket::UnconnectedState) || (socketState == QAbstractSocket::ClosingState))
    {
        //QTcpSocket* sender = static_cast<QTcpSocket*>(QObject::sender());
        clientSocket = nullptr;
        emit onClientDisconnected();
    }
}

void TcpUdp::tcp_onReadyRead()
{
    QTcpSocket* sender = static_cast<QTcpSocket*>(QObject::sender());
    emit onReceived(sender->readAll());
}


void TcpUdp::tcp_close(){

    if(clientSocket != nullptr)
    {
        clientSocket->close();
        clientSocket->deleteLater();
    }
    tcpServer->close();
}

void TcpUdp::tcp_send(QByteArray bytes){

    if(clientSocket != nullptr)
        clientSocket->write(bytes);
}

QString TcpUdp::udp_open()
{
    bool result =  udpSocket->bind(QHostAddress(LOCAL_ADDRESS), UDP_PORT_LOCAL);
    if(!result){
        return "FAIL";
    }
    processPendingDatagrams();
    connect(udpSocket, SIGNAL(readyRead()), this, SLOT(processPendingDatagrams()),Qt::QueuedConnection);
    return "OK";
}

void TcpUdp::udp_close()
{
    udpSocket->close();
}

void TcpUdp::udp_send(QByteArray bytes)
{
    udpSocket->writeDatagram(bytes, QHostAddress(REMOTE_ADDRESS), UDP_PORT_REMOTE);
}

void TcpUdp::processPendingDatagrams()
{
    QHostAddress sender;
    uint16_t port;
    while (udpSocket->hasPendingDatagrams()){
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());
        udpSocket->readDatagram(datagram.data(),datagram.size(),&sender,&port);
        emit onReceived(datagram);
    }
}

#include "TcpClient.h"
#include <QDebug>
#include <QHostAddress>

#define BASIC_STR(x) #x
#define STR(x) BASIC_STR(x)

TcpClient::TcpClient(QObject *parent) : QObject(parent){
}

QByteArray TcpClient::TcpClientConnect_GetAllReceivedData(){
    QByteArray received_data = socket.readAll();
    return received_data;
}

QString TcpClient::TcpClientConnect_GetAddressAndPort(){
    QString addr_port = ("TCP address: " + QString::fromStdString(ADDRESS) + " port: " + QString::number(PORT));
    return addr_port;
}

QTcpSocket * TcpClient::TcpClientConnect_GetSocket(){
    return &socket;
}

bool TcpClient::TcpClient_Connect(){

    socket.connectToHost(QHostAddress(ADDRESS), PORT);

    if(!socket.waitForConnected(5000)){
        qDebug() << socket.errorString();

        return false;
    }
    return true;
}

void TcpClient::TcpClient_Disconnect(){

    socket.close();

}

void TcpClient::TcpClient_Transmit(QByteArray bytes){

    socket.write(bytes);

}

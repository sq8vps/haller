#ifndef TCPUDP_H
#define TCPUDP_H

#include <QObject>
#include <QTcpSocket>
#include <QAbstractSocket>
#include <QUdpSocket>
#include <QDebug>
#include <string>
#include <QString>
#include <QTcpServer>

#define LOCAL_ADDRESS   "192.168.194.89"
#define REMOTE_ADDRESS "192.168.194.91"
//#define ADDRESS     "192.168.1.37"
#define UDP_PORT_LOCAL 25565
#define UDP_PORT_REMOTE 25565
#define TCP_PORT_LOCAL 25563

class TcpUdp : public QObject
{
    Q_OBJECT
public:
    explicit TcpUdp(QObject *parent = nullptr);

    QString tcp_listen(void);
    void tcp_close(void);
    void tcp_send(QByteArray bytes);
    QString udp_open();
    void udp_close();
    void udp_send(QByteArray bytes);

signals:
    void onReceived(QByteArray data);
    void onClientConnected();
    void onClientDisconnected();

public slots:
    void tcp_onStateChanged(QAbstractSocket::SocketState socketState);
    void tcp_onReadyRead();
    void processPendingDatagrams();
    void tcp_onNewConnection(void);

private:
    QTcpServer *tcpServer = nullptr;
    QUdpSocket *udpSocket = nullptr;
    QTcpSocket *clientSocket = nullptr;
};

#endif // TCPCLIENTSOCKET_H

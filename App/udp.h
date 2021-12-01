#ifndef UDP_H
#define UDP_H

#include <QObject>
#include <QUdpSocket>
#include <QAbstractSocket>
#include <QDebug>
#include <string>
#include <QString>

#define ADDRESS     "169.254.69.69"
#define PORT        4242


class Udp: public QObject
{
    Q_OBJECT
public:
    explicit Udp(QObject *parent = nullptr);

    bool Udp_Connect(void);
    void Udp_Disconnect(void);
    void processPendingDatagrams();

private:
    QUdpSocket socket;

};

#endif // UDP_H

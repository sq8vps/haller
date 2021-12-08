#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QHostAddress>

class Client : public QObject
{
    Q_OBJECT

public:
    Q_PROPERTY(bool connected READ getConnected NOTIFY connectedChanged)

    Client(QObject* parent = nullptr) :
        QObject(parent),
        connected(false)
    {
        makeConnections();
    }

    ~Client() override final
    {
        server.disconnectFromHost();
    }

public slots:
    inline void connectToServer(const QString& _address, const int _port)
    {
        server.connectToHost(_address, static_cast<quint16>(_port));
    }

    inline void disconnectFromServer()
    {
        server.disconnectFromHost();
    }

    inline void writeToServer(const QString& _data)
    {
        server.write(_data.toUtf8());
    }

    inline bool getConnected() const
    {
        return connected;
    }

    inline void setConnected(bool _value)
    {
        connected = _value;
        emit connectedChanged();
    }

signals:
    void messageFromServer(const QString& _data);
    void connectedChanged();

private:
    bool connected;
    QTcpSocket server;

    inline void makeConnections()
    {
        // QAbstractSocket
        connect(&server, &QAbstractSocket::connected, [&](){ qDebug() << "Connected!"; setConnected(true); });
        connect(&server, &QAbstractSocket::disconnected, [&](){ qDebug() << "Disconnected!"; setConnected(false); });
        connect(&server, &QAbstractSocket::hostFound, [](){ qDebug() << "Host Found!"; });
        connect(&server, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), [](QAbstractSocket::SocketError _socketError){ qDebug() << "Error occured:" << _socketError; });
        connect(&server, &QAbstractSocket::stateChanged, [](QAbstractSocket::SocketState _socketState){ qDebug() << "Socket state changed to: " << _socketState; });

        // QIODevice
        connect(&server, &QIODevice::readyRead, [&](){ emit messageFromServer(server.readAll()); });
    }
};

#endif // CLIENT_H

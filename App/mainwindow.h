#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "TcpClient.h"
#include <QUdpSocket>

#define ADDRESS     "169.254.69.69"
#define PORT        4242

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void addToLogs(QString message);
    void TcpNewConnectionLogs();
    void TcpNewDisconnectionLogs();
    void TcpNewTcpReceiveLogs();

    void on_Connect_clicked();

    void on_Disconnect_clicked();
    void processPendingDatagrams();
    void on_clear_clicked();

    void on_ConnectUdp_clicked();

    void on_DisconnectUdp_clicked();

    void on_stopMotors_clicked();

    void on_CommitMotors_clicked();

    void on_close_clicked();

private:
    Ui::MainWindow *ui;
    TcpClient rov_tcp_client;
    QUdpSocket *socket=nullptr;;
};
#endif // MAINWINDOW_H

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "TcpUdp.h"



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
    void on_Connect_clicked();
    void on_Disconnect_clicked();
    void on_clear_clicked();
    void on_ConnectUdp_clicked();
    void on_DisconnectUdp_clicked();
    void on_stopMotors_clicked();
    void on_CommitMotors_clicked();
    void on_close_clicked();

    void onReceived(QByteArray data);
    void onClientConnected();
    void onClientDisconnected();

private:
    void addToLogs(QString message);
    Ui::MainWindow *ui;
    TcpUdp tcpUdp;
};
#endif // MAINWINDOW_H

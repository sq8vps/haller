#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "TcpUdp.h"
#include <QTimer>


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

    void on_horizontalSlider_valueChanged(int value);

    void on_horizontalSlider_2_valueChanged(int value);

    void on_horizontalSlider_3_valueChanged(int value);

    void on_horizontalSlider_4_valueChanged(int value);

    void on_horizontalSlider_5_valueChanged(int value);

    void on_horizontalSlider_6_valueChanged(int value);

    void timerTick();

    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_closeGripper_clicked();

    void on_openGripper_clicked();

    void on_torpedo_left_clicked();

    void on_torpedo_right_clicked();

private:
    void addToLogs(QString message);
    Ui::MainWindow *ui;
    TcpUdp tcpUdp;
    void sendServo(int no, int val);
    QTimer *timer;
    enum RampMode
    {
        RAMP_MOTORS,
        RAMP_SERVOS,
        RAMP_BOTH
    };
    enum RampMode rampMode;
};
#endif // MAINWINDOW_H

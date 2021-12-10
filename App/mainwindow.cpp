#include "mainwindow.h"
#include "TcpUdp.h"
#include "ui_mainwindow.h"
#include <../auvConfig/CommunicationCodes.h>


MainWindow::MainWindow(QWidget *parent):
QMainWindow(parent),
ui(new Ui::MainWindow) {

  ui->setupUi(this);
  connect(&tcpUdp, SIGNAL(onClientConnected()), this, SLOT(onClientConnected()));
  connect(&tcpUdp, SIGNAL(onClientDisconnected()), this, SLOT(onClientDisconnected()));
  connect(&tcpUdp, SIGNAL(onReceived(QByteArray)), this, SLOT(onReceived(QByteArray)));
}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::addToLogs(QString message){
    ui->textEdit->append(message);
}

void MainWindow::onClientConnected()
{
    this->addToLogs("Client connected");
}

void MainWindow::onClientDisconnected()
{
    this->addToLogs("Client disconnected");
}

void MainWindow::onReceived(QByteArray data)
{
    this->addToLogs("Data received: " + QString(data));
    int module_id = static_cast<int>(data[0]);

    switch (module_id)
    {

    }
}


void MainWindow::on_Connect_clicked(){
    this->addToLogs("Starting TCP server...");
    this->addToLogs(tcpUdp.tcp_listen());
}


void MainWindow::on_Disconnect_clicked(){
    this->addToLogs("Stopping TCP server... ");
    tcpUdp.tcp_close();
    this->addToLogs("OK");

}


void MainWindow::on_clear_clicked(){
    ui->textEdit->clear();
}


void MainWindow::on_ConnectUdp_clicked()
{
    this->addToLogs("Opening UDP socket...");
    this->addToLogs(tcpUdp.udp_open());
}


void MainWindow::on_DisconnectUdp_clicked()
{
    this->addToLogs("Closing UDP socket... ");
    tcpUdp.udp_close();
    this->addToLogs("OK");
}




void MainWindow::on_CommitMotors_clicked(){
    int pom=0;
    this->addToLogs("Sending motor commands...");
    int motor_torque[5];
    motor_torque[0] = ui->torque1->text().toInt();
    motor_torque[1] = ui->torque2->text().toInt();
    motor_torque[2] = ui->torque3->text().toInt();
    motor_torque[3] = ui->torque4->text().toInt();
    motor_torque[4] = ui->torque5->text().toInt();
    QVarLengthArray<quint8> bytes;
        bytes.clear();
    for(int i=0;i<5;i++){
        if (motor_torque[i] < 0 || motor_torque[i] > 2000){
            this->addToLogs("Wrong values of torques");
            pom=1;
            break;
        }
        }
    if(pom==0){
        // construct motor control mesage
        bytes.append(NORESPREQ_SET_THRUSTERS); // steval id
        bytes.append(5); // payload size
        for(int i=0;i<5;i++){
            quint16 value = static_cast<quint16>(motor_torque[i]);
            bytes.append(value & 0xFF);
            bytes.append((value & 0xFF00) >> 8);
        }

        // convet to QByteArray
        QByteArray motor_control_message;
            motor_control_message.clear();
        for (int i =0; i < bytes.length();i++){
            motor_control_message.append(static_cast<char>(bytes[i]));
        }
        tcpUdp.udp_send(motor_control_message);
        //socket->write(motor_control_message,motor_control_message.size());
    }
}


void MainWindow::on_stopMotors_clicked()
{
    this->addToLogs("Stopping all motors but at this moment i'm doing nothing :)");
    QVarLengthArray<quint8> bytes;
    bytes.clear();

    // construct motor control mesage
        bytes.append(NORESPREQ_SET_THRUSTERS);
        bytes.append(5); // payload size
        for (int i=0;i<5;i++)
        {
            bytes.append(0xe8);
            bytes.append(0x03);
        }
        // convet to QByteArray
        QByteArray motor_control_message;
        motor_control_message.clear();
        for (int i =0; i < bytes.length(); i++)
        {
            motor_control_message.append(static_cast<char>(bytes[i]));
        }
       // tcpUdp.tcp_send(motor_control_message);
        tcpUdp.udp_send(motor_control_message);
}

void MainWindow::on_close_clicked()
{
    close();
}

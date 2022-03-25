#include "mainwindow.h"
#include "TcpClient.h"
#include "ui_mainwindow.h"
#include <modules.h>


MainWindow::MainWindow(QWidget *parent):
QMainWindow(parent),
ui(new Ui::MainWindow) {

  ui->setupUi(this);

}

MainWindow::~MainWindow(){
    delete ui;
}


void MainWindow::addToLogs(QString message){
    ui->textEdit->append(message);
}

void MainWindow::TcpNewConnectionLogs(){
    this->addToLogs("Connected!");
}

void MainWindow::TcpNewDisconnectionLogs(){
    this->addToLogs("Disconnected!");
}

void MainWindow::TcpNewTcpReceiveLogs(){
   // QString log;
    QByteArray received_data = rov_tcp_client.TcpClientConnect_GetAllReceivedData();
//    QString received_data_log = "Data received: " + received_data.toStdString();
//    this->addToLogs(received_data_log);

    int module_id = static_cast<int>(received_data[0]);

    switch (module_id){
    }
}

void MainWindow::processPendingDatagrams(){
    QHostAddress sender;
    uint16_t port;
    while (socket->hasPendingDatagrams()){
        QByteArray datagram;
        datagram.resize(socket->pendingDatagramSize());
        socket->readDatagram(datagram.data(),datagram.size(),&sender,&port);
        addToLogs("Message From :: " + sender.toString());
        addToLogs("Port From :: " + PORT);
        addToLogs("Message :: " + datagram);
    }
}

void MainWindow::on_Connect_clicked(){
    connect(rov_tcp_client.TcpClientConnect_GetSocket(), SIGNAL(connected()), this, SLOT(TcpNewConnectionLogs()));
    connect(rov_tcp_client.TcpClientConnect_GetSocket(), SIGNAL(disconnected()), this, SLOT(TcpNewDisconnectionLogs()));
    connect(rov_tcp_client.TcpClientConnect_GetSocket(), SIGNAL(readyRead()), this, SLOT(TcpNewTcpReceiveLogs()));
    this->addToLogs("Connecting by Tcp...");
    rov_tcp_client.TcpClient_Connect();
}


void MainWindow::on_Disconnect_clicked(){
    this->addToLogs("Closing...");
    rov_tcp_client.TcpClient_Disconnect();
}


void MainWindow::on_clear_clicked(){
    ui->textEdit->clear();
}


void MainWindow::on_ConnectUdp_clicked(){
    socket = new QUdpSocket(this);
    bool result =  socket->bind(QHostAddress(ADDRESS), PORT);
    if(!result){
        addToLogs("FAIL");
    }
    processPendingDatagrams();
    connect(socket, SIGNAL(readyRead()), this, SLOT(processPendingDatagrams()),Qt::QueuedConnection);
    this->addToLogs("Connecting by Udp...");
}


void MainWindow::on_DisconnectUdp_clicked(){
    socket->close();
    this->addToLogs("Disconnected!");
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
        bytes.append(MODULE_STEVAL); // steval id
        bytes.append(10); // payload size
        for(int i=0;i<5;i++){
        quint16 value = static_cast<quint16>(motor_torque[i]);
        bytes.append(value);
        }

        // convet to QByteArray
        QByteArray motor_control_message;
            motor_control_message.clear();
        for (int i =0; i < bytes.length();i++){
            motor_control_message.append(static_cast<char>(bytes[i]));
        }
        socket->writeDatagram(motor_control_message,QHostAddress(ADDRESS),PORT);
        //socket->write(motor_control_message,motor_control_message.size());
    }
}


void MainWindow::on_stopMotors_clicked()
{
    this->addToLogs("Stopping all motors but at this moment i'm doing nothing :)");
    QVarLengthArray<quint8> bytes;
    bytes.clear();

    // construct motor control mesage
        bytes.append(MODULE_STEVAL); // motor module M4 handler id
        bytes.append(10); // payload size
        for (int i=0;i<10;i++)
            bytes.append(0); //unused, but payload size must be equal to 10

        // convet to QByteArray
        QByteArray motor_control_message;
        motor_control_message.clear();
        for (int i =0; i < bytes.length(); i++)
        {
            motor_control_message.append(static_cast<char>(bytes[i]));
        }

}

void MainWindow::on_close_clicked()
{
    close();
}


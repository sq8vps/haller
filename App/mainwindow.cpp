#include "mainwindow.h"
#include "TcpUdp.h"
#include "ui_mainwindow.h"
#include <../auvConfig/CommunicationCodes.h>
#include <math.h>


MainWindow::MainWindow(QWidget *parent):
QMainWindow(parent),
ui(new Ui::MainWindow) {

  ui->setupUi(this);
  connect(&tcpUdp, SIGNAL(onClientConnected()), this, SLOT(onClientConnected()));
  connect(&tcpUdp, SIGNAL(onClientDisconnected()), this, SLOT(onClientDisconnected()));
  connect(&tcpUdp, SIGNAL(onReceived(QByteArray)), this, SLOT(onReceived(QByteArray)));
  timer = new QTimer(this);
  connect(timer, &QTimer::timeout, this, &MainWindow::timerTick);
  rampMode = RAMP_MOTORS;
}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::timerTick()
{
    if(rampMode == RAMP_MOTORS || rampMode == RAMP_BOTH)
    {
        QVarLengthArray<quint8> bytes;
            bytes.clear();

            static float arg = 0.f;
            arg += 3.14f / 20.f;
            if(arg > 6.28f)
                arg = 0.f;

            // construct motor control mesage
            bytes.append(NORESPREQ_SET_THRUSTERS); // steval id
            bytes.append(5); // payload size
            quint16 value = static_cast<quint16>((sinf(arg) + 1.f) * 1000.f);
            for(int i=0;i<5;i++){

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

    if(rampMode == RAMP_SERVOS || rampMode == RAMP_BOTH)
    {
        QVarLengthArray<quint8> bytes;


        static float arg = 0.f;
        arg += 3.14f / 20.f;
        if(arg > 6.28f)
            arg = 0.f;

        for(quint8 j = 0; j < 6; j++)
        {
            bytes.clear();
            bytes.append(NORESPREQ_SET_SERVOS);
            bytes.append(2); // payload size
            bytes.append(j);
            bytes.append(0);
            bytes.append(static_cast<quint8>((sinf(arg) + 1.f) * 50.f));
            bytes.append(0);
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
    }
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
    //this->addToLogs("Data received: " + QString(data));
    int module_id = static_cast<int>(data[0]);

    switch (module_id)
    {
       /* case 'pressure':
        quint32 val = 0;
        val |= data[5];
        val <<= 8;
        val |= data[4];
        val <<= 8;
        val |= data[3];
        val <<= 8;
        val |= data[2];
        val /= 100;
        this->addToLogs("Pressure data: " + QString::number(val));
        break;
        */
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

void MainWindow::sendServo(int no, int val)
{
    this->addToLogs("Setting servo to " + QString::number(val));
    QVarLengthArray<quint8> bytes;
    bytes.clear();

    // construct motor control mesage
        bytes.append(NORESPREQ_SET_SERVOS);
        bytes.append(2); // payload size
        bytes.append(static_cast<quint8>(no));
        bytes.append(0);
        bytes.append(static_cast<quint8>(val));
        bytes.append(0);
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


void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    sendServo(0, value);
}

void MainWindow::on_horizontalSlider_2_valueChanged(int value)
{
    sendServo(1, value);
}

void MainWindow::on_horizontalSlider_3_valueChanged(int value)
{
    sendServo(2, value);
}

void MainWindow::on_horizontalSlider_4_valueChanged(int value)
{
    sendServo(3, value);
}

void MainWindow::on_horizontalSlider_5_valueChanged(int value)
{
    sendServo(4, value);
}

void MainWindow::on_horizontalSlider_6_valueChanged(int value)
{
    sendServo(5, value);
}

void MainWindow::on_pushButton_2_clicked()
{
    timer->start(ui->lineEdit->text().toInt());
    rampMode = RAMP_SERVOS;
}

void MainWindow::on_pushButton_clicked()
{
    timer->start(ui->lineEdit->text().toInt());
    rampMode = RAMP_MOTORS;
}

void MainWindow::on_pushButton_3_clicked()
{
    timer->stop();
}

void MainWindow::on_pushButton_4_clicked()
{
    ui->horizontalSlider->setValue(50);
    ui->horizontalSlider_2->setValue(50);
    ui->horizontalSlider_3->setValue(50);
    ui->horizontalSlider_4->setValue(50);
    ui->horizontalSlider_5->setValue(50);
    ui->horizontalSlider_6->setValue(50);
    sendServo(0, 50);
    sendServo(1, 50);
    sendServo(2, 50);
    sendServo(3, 50);
    sendServo(4, 50);
    sendServo(5, 50);
}

void MainWindow::on_pushButton_5_clicked()
{
    timer->start(ui->lineEdit->text().toInt());
    rampMode = RAMP_BOTH;
}

void MainWindow::on_pushButton_6_clicked()
{
    QVarLengthArray<quint8> bytes;
    bytes.clear();
    // construct motor control mesage
        bytes.append(NORESPREQ_SET_SERVOS);
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


void MainWindow::on_pushButton_7_clicked()
{
    this->addToLogs("Setting pressure");
    QVarLengthArray<quint8> bytes;
    bytes.clear();
    int val=ui->lineEdit_3->text().toInt();

    // construct motor control mesage
        bytes.append(NORESPREQ_SET_SERVOS);
        bytes.append(1); // payload size
        bytes.append(static_cast<quint8>(val & 0xFF));
        bytes.append(static_cast<quint8>((val >> 8) & 0xFF));
        // convet to QByteArray
        QByteArray data;
        data.clear();
        for (int i =0; i < bytes.length(); i++)
        {
            data.append(static_cast<char>(bytes[i]));
        }
       // tcpUdp.tcp_send(motor_control_message);
        tcpUdp.udp_send(data);

}


void MainWindow::on_closeGripper_clicked()
{
    QVarLengthArray<quint16> bytes;
    bytes.clear();

    // construct motor control mesage
        bytes.append(NORESPREQ_CLOSE_GRIPPER);
        bytes.append(0); // payload size
        // convet to QByteArray
        QByteArray data;
        data.clear();
        for (int i =0; i < bytes.length(); i++)
        {
            data.append(static_cast<char>(bytes[i]));
        }
       // tcpUdp.tcp_send(motor_control_message);
        tcpUdp.udp_send(data);
}


void MainWindow::on_openGripper_clicked()
{
    QVarLengthArray<quint16> bytes;
    bytes.clear();

    // construct motor control mesage
        bytes.append(NORESPREQ_OPEN_GRIPPER);
        bytes.append(0); // payload size
        // convet to QByteArray
        QByteArray data;
        data.clear();
        for (int i =0; i < bytes.length(); i++)
        {
            data.append(static_cast<char>(bytes[i]));
        }
       // tcpUdp.tcp_send(motor_control_message);
        tcpUdp.udp_send(data);
}


void MainWindow::on_torpedo_left_clicked()
{

    QVarLengthArray<quint16> bytes;
    bytes.clear();

    // construct motor control mesage
        bytes.append(NORESPREQ_LAUNCH_TORPEDO);
        bytes.append(1); // payload size
        bytes.append(static_cast<quint16>(1 & 0xFF));
        // convet to QByteArray
        QByteArray data;
        data.clear();
        for (int i =0; i < bytes.length(); i++)
        {
            data.append(static_cast<char>(bytes[i]));
        }
       // tcpUdp.tcp_send(motor_control_message);
        tcpUdp.udp_send(data);

}


void MainWindow::on_torpedo_right_clicked()
{

    QVarLengthArray<quint16> bytes;
    bytes.clear();

    // construct motor control mesage
        bytes.append(NORESPREQ_LAUNCH_TORPEDO);
        bytes.append(1); // payload size
        bytes.append(static_cast<quint16>(1 & 0xFF));
        // convet to QByteArray
        QByteArray data;
        data.clear();
        for (int i =0; i < bytes.length(); i++)
        {
            data.append(static_cast<char>(bytes[i]));
        }
       // tcpUdp.tcp_send(motor_control_message);
        tcpUdp.udp_send(data);
}


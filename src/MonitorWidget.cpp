//
// Created by Mingirsu on 2024/1/11.
//
#include <iostream>

#include "QLabel"
#include "QVBoxLayout"
#include "QGridLayout"
#include "QDoubleSpinBox"

#include "MonitorWidget.h"

MonitorWidget::MonitorWidget(QWidget *parent) :
        m_info_timer(new QTimer),
        m_tcp_socket(new QTcpSocket),
        m_connect_button(new QPushButton),
        m_set_button(new QPushButton) {
    setUpUi();

    //发生某些事件触发的信号
    connect(m_tcp_socket, SIGNAL(connected), this, SLOT(onConnect));
    connect(m_tcp_socket, SIGNAL(disconnected), this, SLOT(onDisconnect));
    //按键信号
    connect(m_connect_button, &QPushButton::clicked, this, SLOT(onActStart));

}

void MonitorWidget::onConnectStatusChange() {
//按钮改变，或远程设备断开连接、网络环境导致的改变
}

void MonitorWidget::onConnect() {
    //显示连接成功，启用QTimer和调节按钮
    std::cout<<"已连接成功！\n"<<std::endl;
}

void MonitorWidget::onDisconnect() {
    //显示断开连接，关闭QTimer和调节，记录日志
}

void MonitorWidget::onActStart() {
    //连接成功发出connected信号
    m_tcp_socket->connectToHost(m_host, m_port);
}

void MonitorWidget::onActStop() {

}

void MonitorWidget::onSetTemperature() {
    //若处于连接
    if (m_tcp_socket->state() == QAbstractSocket::ConnectedState) {

    }else{
        std::cout<<"设备未连接，请检查连接状态\n"<<std::endl;
    }
    //Out1.PID.setpoint
    //发送消息后，设备是否有相应的回复？
}

void MonitorWidget::setUpUi() {
    auto entire_layout = new QGridLayout;
    auto *layout = new QVBoxLayout(this);
    auto *temperature_label = new QLabel("Temperature: ", this);
    auto *voltage_label = new QLabel("Voltage: ", this);
    layout->addWidget(temperature_label);
    layout->addWidget(voltage_label);
    auto *connect_button = new QPushButton("Connect");


    entire_layout->addWidget(temperature_label, 0, 0);

    entire_layout->addWidget(voltage_label, 1, 0);

    entire_layout->addWidget(m_connect_button, 2, 1);

    entire_layout->addWidget(m_set_button, 3, 1);
}


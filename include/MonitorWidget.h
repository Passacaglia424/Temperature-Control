//
// Created by Mingirsu on 2024/1/11.
//

#ifndef TEMPERATURE_CONTROL_MONITORWIDGET_H
#define TEMPERATURE_CONTROL_MONITORWIDGET_H

#include "QWidget"
#include "QTcpSocket"
#include "QTcpServer"
#include "QHostAddress"
#include "QTimer"
#include "QPushButton"

class MonitorWidget : public QWidget{
    Q_OBJECT
public:
    explicit MonitorWidget(QWidget *parent= nullptr);
    ~MonitorWidget() override = default;

signals:
    void sigConnectRequest(bool is_succeed);

public slots:
    /**
     * @brief 连接状态改变触发
     */
    void onConnectStatusChange();

    /**
     * @brief 连接上时触发，在ui显示
     */
     void onConnect();

    /**
     * @brief 断开连接时触发，在ui显示。
     */
     void onDisconnect();

     /**
      * 与连接按钮相关
      */
     void onActStart();

     void onActStop();

     /**
      * @brief 设置温度的按钮，发送调温宏。
      */
     void onSetTemperature();

private:
    void setUpUi();

    int m_port = 23;
    QString m_host="10.108.14.216";
    double m_temperature = 30.0;
    QTimer *m_info_timer;
    QTcpSocket *m_tcp_socket;
    QPushButton *m_connect_button;
    QPushButton *m_set_button;

};

#endif //TEMPERATURE_CONTROL_MONITORWIDGET_H

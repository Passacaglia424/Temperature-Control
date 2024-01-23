//
// Created by Mingirsu on 2024/1/11.
//

#ifndef TEMPERATURE_CONTROL_MONITORWIDGET_H
#define TEMPERATURE_CONTROL_MONITORWIDGET_H

#include <string>
#include "QWidget"
#include "QTcpSocket"
#include "QTcpServer"
#include "QHostAddress"
#include "QTimer"
#include "QPushButton"
#include "QDoubleSpinBox"
#include "QSpinBox"
#include "QVector"
#include "QCheckBox"
#include "QQueue"

#include "util/path_browser.h"
#include "util/plot_widget.h"
#include "ThreadManager.h"

class MonitorWidget : public QWidget {
Q_OBJECT

public:
    explicit MonitorWidget(QWidget *parent = nullptr);

    ~MonitorWidget() override = default;

signals:

    void sigConnectRequest(bool is_succeed);

    void sigRequestCurrentInfo(QTcpSocket *tcp_socket);

public slots:

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

    /**
     * @brief 每当收到socket的消息时触发。解析收到的消息
     */
    void onReceivedInfo();

    /*每隔采样时间询问一次*/
    void onQueryInfo();

private:
    void setUpUi();

    int m_port = 40;
    QString m_host = "10.108.14.216";
    //QString m_host = "10.62.153.149";
    double m_temperature = 30.0000;
    std::string m_channel = "2A";
    std::string m_output = "Out3";
    QVector<double>m_temperature_assemble;
    QVector<double>m_voltage_assemble;

    QQueue<QDateTime>m_time_queue;
    QString m_save_path; //当前保存的文件路径
    QString m_save_file; //当前保存的文件名
    int m_sampling_time = 1;//采样时间，单位:秒
    int m_max_point = 1000;
    QTimer *m_info_timer; //计时器
    QTcpSocket *m_tcp_socket;
    ThreadManager *m_thread_manager;

    QLabel *m_temperature_show;
    QLabel *m_voltage_show;
    GraphWidget *m_plot_voltage;
    GraphWidget *m_plot_temperature;
    QPushButton *m_connect_button;
    QPushButton *m_set_button;
    QDoubleSpinBox *m_temperature_input;
    QSpinBox *m_sampling_input;
    PathBrowser *m_path_browser;
    QLabel *m_connect_status;
    QCheckBox *m_auto_save_box;
};

#endif //TEMPERATURE_CONTROL_MONITORWIDGET_H

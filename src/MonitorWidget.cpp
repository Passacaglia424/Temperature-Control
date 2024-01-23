//
// Created by Mingirsu on 2024/1/11.
//
#include <iostream>
#include <sstream>

#include "QLabel"
#include "QVBoxLayout"
#include "QGridLayout"
#include "QFile"
#include "QDir"
#include "QMessageBox"

#include "MonitorWidget.h"

MonitorWidget::MonitorWidget(QWidget *parent) :
        m_info_timer(new QTimer),
        m_tcp_socket(new QTcpSocket),
        m_plot_voltage(new GraphWidget),
        m_plot_temperature(new GraphWidget),
        m_connect_button(new QPushButton("连接设备")),
        m_set_button(new QPushButton("更改温度")),
        m_temperature_input(new QDoubleSpinBox),
        m_sampling_input(new QSpinBox),
        m_connect_status(new QLabel("未连接")),
        m_temperature_show(new QLabel()),
        m_voltage_show(new QLabel()),
        m_path_browser(new PathBrowser),
        m_auto_save_box(new QCheckBox("Auto Save")) {
    setUpUi();
    //m_thread_manager = ThreadManager::getInstance();
    //发生某些事件触发的信号
    connect(m_tcp_socket, SIGNAL(connected()), this, SLOT(onConnect()));
    connect(m_tcp_socket, SIGNAL(disconnected()), this, SLOT(onDisconnect()));
    connect(m_tcp_socket, &QTcpSocket::readyRead, this, [this]() {
        onReceivedInfo();
    });
    connect(m_sampling_input, &QSpinBox::valueChanged, this, [this]() {
        m_info_timer->setInterval(m_sampling_input->value() * 1000);
    });
    connect(m_info_timer, &QTimer::timeout, this, [this]() {
        onQueryInfo();
    });
    //按键信号。
    connect(m_connect_button, &QPushButton::clicked, this, [this]() {
        onActStart();
    });
    connect(m_set_button, &QPushButton::clicked, this, [this]() {
        onSetTemperature();
    });
    m_info_timer->start((m_sampling_input->value() * 1000));

}

void MonitorWidget::onConnect() {
    //显示连接成功，启用调节按钮
    std::cout << "Connection succeeds.\n" << std::endl;
    m_connect_status->setText("已连接");
    m_connect_button->setText("断开连接");
    QDir save_folder(m_save_path);
    auto save_path = QDir::cleanPath(
            save_folder.absolutePath() + QDir::separator() + QDateTime::currentDateTime().toString("yyyy-MM-dd"));
    if (!save_folder.mkpath(save_path)) {
        QMessageBox::warning(this, "保存文件", "创建文件夹失败，请检查是否有写入权限。");
        return;
    }
    m_save_file = QDir::cleanPath(save_path) + QDir::separator() +
                  QDateTime::currentDateTime().toString("yyyy-MM-dd-hh-mm-ss") + ".txt";
    QFile save_file(m_save_file);
    if (!save_file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "保存文件", "打开文件失败，请检查是否有写入权限。");
        return;
    }
    save_file.write("     Time          Temperature       Voltage\n");
}

void MonitorWidget::onDisconnect() {
    //显示断开连接，关闭QTimer和调节，记录日志
    std::cout << "Connection down.\n" << std::endl;
    //m_thread_manager->deleteThread("监听温度");
    m_temperature_show->setText("");
    m_voltage_show->setText("");
    m_connect_status->setText("未连接");
    m_connect_button->setText("连接");
    m_time_queue.clear();

}

void MonitorWidget::onActStart() {
    //连接成功发出connected信号，断开发出disconnected信号。
    m_connect_button->setEnabled(false);
    if (m_tcp_socket->state() == QAbstractSocket::ConnectedState) {
        m_tcp_socket->disconnectFromHost();
    } else {
        m_connect_status->setText("连接中……");
        m_tcp_socket->connectToHost(m_host, m_port);
        if (!m_tcp_socket->waitForConnected(10000)) {
            m_connect_status->setText("未连接");
            std::cout << "Connection fails." << std::endl;
        }
    }
    m_connect_button->setEnabled(true);
}

void MonitorWidget::onActStop() {

}

void MonitorWidget::onReceivedInfo() {
    m_tcp_socket->waitForReadyRead(1000); //确保消息传完
    QByteArray data = m_tcp_socket->readAll();
    std::string info = data.toStdString();
    std::string temp_string;
    std::stringstream info_stream(info);
    //消息处理
    QDateTime time;
    if (!m_time_queue.empty()) {
        time = m_time_queue.front();
        m_time_queue.pop_front();
    }else{
        return;
    }
    double key = time.toMSecsSinceEpoch() / 1000.0;
    while (info_stream >> temp_string) {
        if (temp_string == m_channel + ".Value") {
            info_stream >> temp_string;
            info_stream >> temp_string;
            m_plot_temperature->appendData(0, key, atof(temp_string.c_str()));
            m_temperature_show->setText(temp_string.c_str());
        } else if (temp_string == m_output + ".Value") {
            info_stream >> temp_string;
            info_stream >> temp_string;
            m_plot_voltage->appendData(0, key, atof(temp_string.c_str()));
            m_voltage_show->setText(temp_string.c_str());
        } else {//报错信息等杂东西
            std::cout << temp_string << std::endl;
        }
    }
    //TODO:删除多久前的数据以方便显示或防内存爆栈?最大显示多少个数据点？
    m_plot_voltage->reScaleAxes();
    m_plot_voltage->replot();
    m_plot_temperature->reScaleAxes();
    m_plot_temperature->replot();
    m_plot_voltage->m_custom_plot->axisRect()->setupFullAxesBox();
    if (m_auto_save_box->isChecked()) {
        QFile file(m_save_file);
        file.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Append);
        file.write(
                (QString::number(time.toSecsSinceEpoch()) + "         " + m_temperature_show->text() + "             " +
                 m_voltage_show->text() + "\n").toUtf8());
        file.close();
    }
}

void MonitorWidget::onQueryInfo() {
    if (m_tcp_socket->state() == QAbstractSocket::ConnectedState) {
        m_tcp_socket->write((m_channel + "?\n").c_str());
        m_tcp_socket->write((m_output + "?\n").c_str());
        m_tcp_socket->flush();
        m_time_queue.push_back(QDateTime::currentDateTime());
    }
}

void MonitorWidget::onSetTemperature() {
    //若处于连接
    if (m_tcp_socket->state() == QAbstractSocket::ConnectedState) {
        m_tcp_socket->write(("Out3.PID.setpoint = " + std::to_string(m_temperature_input->value()) + "\n").c_str());
        m_tcp_socket->flush();
    } else {
        std::cout << "Device disconnects, please check out the connection status.\n" << std::endl;
    }
}

void MonitorWidget::setUpUi() {
    auto entire_layout = new QGridLayout(this);
    auto plot_layout = new QGridLayout;
    auto plot_layout_up = new QGridLayout;
    auto setting_layout = new QGridLayout;
    auto *temperature_label_fix = new QLabel("Current Temperature(℃): ", this);
    auto *voltage_label_fix = new QLabel("Current Voltage(V): ", this);
    auto *sampling_label_fix = new QLabel("Sampling Time(s): ");

    m_temperature_input->setValue(m_temperature);
    m_temperature_input->setDecimals(3);
    m_temperature_input->setSingleStep(0.1);
    m_temperature_input->setMinimum(-273.15);
    m_sampling_input->setValue(10);
    m_sampling_input->setMinimum(1);
    m_auto_save_box->setChecked(true);
    m_path_browser->setPath("./");
    //TODO:准备一个配置文件。用户选择新的保存地址时，将该保存地址写入配置文件。下次从这里读取。如果读不到再使用默认地址。

    //暂时不调温
    m_set_button->setDisabled(true);
    m_plot_voltage->addGraph();
    m_plot_temperature->addGraph();

    //电压信息框
    m_plot_voltage->setBrush(0, Qt::NoBrush);
    m_plot_voltage->setPen(0, QPen("#32CD99"));
    m_plot_voltage->setLayer(0, "overlay");
    QCPScatterStyle scatterStyle;
    scatterStyle.setBrush(QBrush("#32CD32"));
    scatterStyle.setPen(QPen("#32CD32"));
    scatterStyle.setSize(2);
    scatterStyle.setShape(QCPScatterStyle::ScatterShape::ssDisc);
    m_plot_voltage->setLegendVisible(true);
    m_plot_voltage->setName(0, "Voltage");
    m_plot_voltage->setScatterStyle(0, scatterStyle);
    m_plot_voltage->setLineStyle(0, QCPGraph::lsLine);
    m_plot_voltage->setLabel(BasicPlotWidget::YLeft, "Voltage / V");
    m_plot_voltage->setLabel(BasicPlotWidget::XBottom, "Time(Data Time)");
    //温度信息框
    m_plot_temperature->setBrush(0, Qt::NoBrush);
    m_plot_temperature->setPen(0, QPen("#FF2400"));
    m_plot_temperature->setLayer(0, "overlay");
    QCPScatterStyle no_highlight;
    no_highlight.setBrush(QBrush("#BC1717"));
    no_highlight.setPen(QPen("BC1717"));
    no_highlight.setSize(2);
    m_plot_temperature->setLegendVisible(true);
    m_plot_temperature->setName(0, "Temperature");
    m_plot_temperature->setScatterStyle(0, no_highlight);
    m_plot_temperature->setLineStyle(0, QCPGraph::lsLine);
    m_plot_temperature->setLabel(BasicPlotWidget::YLeft, "Temperature / °C");
    m_plot_temperature->setLabel(BasicPlotWidget::XBottom, "Time(Data Time)");
    //m_plot_voltage->m_custom_plot->yAxis->setNumberPrecision(100);
    //实时横坐标
    QSharedPointer<QCPAxisTickerDateTime> timer(new QCPAxisTickerDateTime);
    timer->setDateTimeFormat("hh:mm:ss");
    timer->setTickCount(10);
    //m_plot_voltage->m_custom_plot->xAxis->setTickLabelRotation(30);//顺时针旋转30度
    //m_plot_temperature->m_custom_plot->xAxis->setTickLabelRotation(30);
    timer->setTickStepStrategy(QCPAxisTicker::tssMeetTickCount);
    m_plot_voltage->m_custom_plot->xAxis->setTicker(timer);
    m_plot_temperature->m_custom_plot->xAxis->setTicker(timer);

    plot_layout_up->addWidget(temperature_label_fix, 0, 0);
    plot_layout_up->addWidget(m_temperature_show, 0, 1);
    plot_layout_up->addWidget(voltage_label_fix, 1, 0);
    plot_layout_up->addWidget(m_voltage_show, 1, 1);
    auto plot_widget_up = new QWidget;
    plot_widget_up->setFixedHeight(60);
    plot_widget_up->setLayout(plot_layout_up);
    plot_layout->addWidget(plot_widget_up, 0, 0);
    plot_layout->addWidget(m_plot_voltage, 1, 0);
    plot_layout->addWidget(m_plot_temperature, 2, 0);
    auto *plot_widget = new QWidget;
    plot_widget->setLayout(plot_layout);

    setting_layout->addWidget(m_temperature_input, 1, 0); ///温度设置相关
    setting_layout->addWidget(m_set_button, 1, 1);
    setting_layout->addWidget(sampling_label_fix, 2, 0);///采样设置相关
    setting_layout->addWidget(m_sampling_input, 2, 1);
    setting_layout->addWidget(m_path_browser, 3, 0, 1, 2);
    setting_layout->addWidget(m_auto_save_box, 4, 0, 1, 2);
    setting_layout->addWidget(m_connect_status, 5, 0); ///连接相关
    setting_layout->addWidget(m_connect_button, 5, 1);
    //setting_layout->setVerticalSpacing(70);

    auto setting_widget = new QWidget;
    setting_widget->setLayout(setting_layout);

    entire_layout->addWidget(plot_widget, 0, 0);
    entire_layout->addWidget(setting_widget, 0, 2);
    entire_layout->setColumnStretch(0, 2);
    this->setLayout(entire_layout);
}


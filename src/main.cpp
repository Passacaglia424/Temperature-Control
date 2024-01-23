#include <iostream>

#include "QApplication"

#include "MonitorWidget.h"

#define MAX_BUFF_SIZE 1024

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    // 创建并显示窗口
    auto *monitor_widget = new MonitorWidget;
    monitor_widget->setMinimumSize(1440, 810);
    monitor_widget->show();

    return a.exec();
}
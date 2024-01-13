#include <iostream>

#include "QApplication"

#include "MonitorWidget.h"
#define MAX_BUFF_SIZE 1024

int main(){
    QApplication a();
    // 创建并显示窗口
    MonitorWidget monitorWidget;
    monitorWidget.show();

    return a.exec();
}
//
// Created by Mingirsu on 2024/1/18.
//

#ifndef TEMPERATURE_CONTROL_THREADMANAGER_H
#define TEMPERATURE_CONTROL_THREADMANAGER_H

#include "QThread"
#include "QMap"

class ThreadManager : public QObject {
public:
    static ThreadManager *getInstance();

    ~ThreadManager() = default;

    /**
     * @brief create a thread named QThread.
     * @return
     */
    QThread* createThread(const QString &name);

    /**
     * @brief name
     * @return whether detele succeeded
     */
    bool deleteThread(const QString &name);

private:
    ThreadManager();

    QMap<QString, QThread *> m_threads;
};


#endif //TEMPERATURE_CONTROL_THREADMANAGER_H

//
// Created by Mingirsu on 2024/1/18.
//
#include "ThreadManager.h"

ThreadManager::ThreadManager() : QObject(){

}

QThread* ThreadManager::createThread(const QString &name) {
    if (m_threads.find(name) != m_threads.end()) {
        return m_threads[name];
    }
    auto *thread = new QThread(this);
    m_threads.insert(name, thread);
    return thread;
}

bool ThreadManager::deleteThread(const QString &name) {
    if(m_threads.find(name) == m_threads.end()){
        return false;
    }
    //如果正在进行某些操作，会等操作结束吗？
    auto thread = m_threads[name];
    thread->quit();
    bool ok = thread->wait(5000);
    m_threads.remove(name);
    thread->deleteLater();
}

ThreadManager* ThreadManager::getInstance() {
    static ThreadManager instance;
    return &instance;
}
#include "task.h"

#include <stdlib.h>
#include <iostream>
using namespace std;

#include <QThread>
#include <QThreadPool>
#include <QDebug>


#include "TGClientSDK.h"

int succeed = 0;
int total = 0;
int failed = 0;

Task::Task()
{

}

void Task::run()
{
    qDebug() << "Active :" << QThreadPool::globalInstance()->activeThreadCount();

    if (!TGClient_Init())
    {
        qDebug() << QThread::currentThreadId() << ": Init failed";
        QThread::currentThread()->exit(-1);
    }

    string ip = "121.201.5.141";
    string name = "IPC_demo";
    string pwd = "123445";

    mutex.lock();
    long handle = TGClient_AsyncLogin((char*)ip.data(), 13000, name.data(), pwd.data(), 144115188075866244, nullptr, nullptr);
    mutex.unlock();
    if (!handle)
    {
        mutex.lock();
        failed++;
        total++;
        mutex.unlock();
    }
    else
    {
        mutex.lock();
        succeed++;
        total++;
        mutex.unlock();
    }
}

#ifndef TASK_H
#define TASK_H

#include <QRunnable>
#include <QMutex>

class Task : public QRunnable
{
public:
    Task();
    void run();

private:
    QMutex mutex;
};

#endif // TASK_H

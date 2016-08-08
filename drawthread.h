#ifndef DRAWTHREAD_H
#define DRAWTHREAD_H

#include <QThread>

class DrawThread : public QThread
{
public:
    DrawThread(QObject* parent = 0);

    void run() override final;

private:
};

#endif // DRAWTHREAD_H

#ifndef DRAWTHREAD_H
#define DRAWTHREAD_H

#include <QThread>

class TGFile;

class DrawThread : public QThread
{
    friend class MainWindow;
public:
    DrawThread(const QString &fileName, QObject* parent = 0);
    ~DrawThread();

    void run() override final;

private:
    bool m_stop;
    TGFile *m_pTGFile;
};

#endif // DRAWTHREAD_H

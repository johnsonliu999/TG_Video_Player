#ifndef DRAWTHREAD_H
#define DRAWTHREAD_H

#include <QThread>

class TGFile;

class DrawThread : public QThread
{
    Q_OBJECT
    friend class MainWindow;

    enum {
        BUFFER_SIZE = 512 * 1024,
        YUV_SIZE = 1920 * 1080 * 3,

    };

public:
    DrawThread(QObject* parent = 0);
    ~DrawThread();

signals:
    void updateTotalTime(const QString &totalTime);
    void updateCurTime(const QString &curTime);

public slots:
    void openFile(const QString &path);
    void on_startPlay();

private:
    TGFile *m_pTGFile;
    quint8 *m_pBuffer;
    quint8 *m_pYuv;

    quint64 m_pts;
    quint64 m_width;
    quint64 m_height;
    quint64 m_decHandle;
    quint64 m_drawHandle;

    quint64 m_curTime;
    quint64 m_totalTime;
};

#endif // DRAWTHREAD_H

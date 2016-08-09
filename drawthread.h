#ifndef DRAWTHREAD_H
#define DRAWTHREAD_H

#include <QThread>

class TGFile;
class QTimer;

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
    void updateStatusBar(const QString &text);

public slots:
    void openFile(const QString &path);
    void on_startPlay();

private slots:
    void on_timeout();

private:
    void processFrame();

    TGFile *m_pTGFile;

    quint8 *m_pBuffer;
    quint8 *m_pYuv;

    bool m_stop;

    quint64 m_pts;
    long m_width;
    long m_height;
    quint64 m_decHandle;
    quint64 m_drawHandle;

    quint64 m_curTime;
    quint64 m_totalTime;

    int m_readSize;
};

#endif // DRAWTHREAD_H

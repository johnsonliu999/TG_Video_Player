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
    void updateSlider(const int &value);

public slots:
    void on_openFile(const QString &path);
    void on_startPlay();
    void on_stopPlay();
    void on_jumpTo(const quint64 time);

private slots:
    void on_timeout();

private:
    void processFrame();

    TGFile *m_pTGFile;

    quint8 *m_pBuffer;
    quint8 *m_pYuv;

    bool m_stop;
    bool m_play;

    quint64 m_pts;
    long m_width;
    long m_height;
    quint64 m_decHandle;
    quint64 m_drawHandle;
    quint64 m_dispHandle;

    quint64 m_totalTime;

    int m_readSize;
};

#endif // DRAWTHREAD_H

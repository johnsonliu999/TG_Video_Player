#include "drawthread.h"

#include "mainwindow.h"
#include "tgfile.h"
#include "PlayMediaSDK.h"

#include <QDebug>
#include <QTimer>


DrawThread::DrawThread(QObject *parent) :
    QThread(parent),
    m_pTGFile(new TGFile),
    m_pBuffer(new quint8[BUFFER_SIZE]),
    m_pYuv(new quint8[YUV_SIZE]),
    m_pts(0),
    m_stop(false)
{
    m_decHandle = PlayMedia_CreateVideoDecodec(0);
}

DrawThread::~DrawThread()
{
    delete m_pTGFile;
    delete m_pBuffer;
    delete m_pYuv;
}

void DrawThread::openFile(const QString &path)
{
    if (!m_pTGFile->open(path)) {
        qDebug() << "Parse file failed";
        return ;
    }

    if (!m_pTGFile->seek(0)) {
        qDebug() << "Seek error";
        return ;
    }

    m_pts = 0;

    // read frame
    FrameInfo frameInfo;
    m_readSize = m_pTGFile->readFrame(m_pBuffer, BUFFER_SIZE, frameInfo);
    if (m_readSize < 0) {
        qDebug() << "Read error";
        return ;
    }

    // decode first frame
    long yuvSize, type;
    if (ERR_SUCCESS != PlayMedia_DecodecVideo(m_decHandle, m_pBuffer, m_readSize, m_pYuv, (long *)&yuvSize, (long *)&type)) {
        qDebug() << "Decode Error";
        return ;
    }

    // get size
    int res = PlayMedia_GetPictureSize(m_decHandle, &m_width, &m_height);
    qDebug() << "res :" << res;
    qDebug() << "width :" << m_width;
    qDebug() << "Height :" << m_height;

    // init draw
    m_drawHandle = PlayMedia_InitDDraw(m_dispHandle, m_width, m_height);

    // update total time display
    m_totalTime = m_pTGFile->getTimeLength();
    if (m_totalTime <= 0) {
        qDebug() << "File length error";
        m_pTGFile->close();
    }
    m_curTime = 0;
    int sec = m_totalTime/1000%60;
    int min = m_totalTime/1000/60%60;
    QString total = QString("%1:%2").arg(min, 2, 10, QLatin1Char('0')).arg(sec, 2, 10, QLatin1Char('0'));
    QString current = QString("00:00");
    emit updateTotalTime(total);
    emit updateCurTime(current);

    emit updateStatusBar(QString("File loaded succeed"));
}

void DrawThread::on_startPlay()
{
    if (!m_pTGFile->isOpen()) {
        qDebug() << "File not open";
        return ;
    }

    m_stop = false;

    processFrame();
}

void DrawThread::on_stopPlay()
{
    m_stop = true;
}

void DrawThread::on_timeout()
{
    long yuvSize, type;
    if (ERR_SUCCESS != PlayMedia_DecodecVideo(m_decHandle, m_pBuffer, m_readSize, m_pYuv, (long *)&yuvSize, (long *)&type)) {
        qDebug() << "Decode Error";
        return ;
    }

    processFrame();
}

inline void DrawThread::processFrame()
{
    PlayMedia_DDraw(m_drawHandle, m_pYuv, m_width, m_height);

    if (m_stop) return;

    if (m_pTGFile->atEnd()) return;
    FrameInfo frameInfo;

    m_readSize = m_pTGFile->readFrame(m_pBuffer, BUFFER_SIZE, frameInfo);
    int waitTime = frameInfo.timestamp - m_pts;
    m_pts = frameInfo.timestamp;

    QTimer::singleShot(waitTime, this, &DrawThread::on_timeout);
}

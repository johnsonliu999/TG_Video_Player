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
    m_stop(true),
    m_play(false)
{
    m_decHandle = PlayMedia_CreateVideoDecodec(0);
}

DrawThread::~DrawThread()
{
    delete m_pTGFile;
    delete m_pBuffer;
    delete m_pYuv;
}

void DrawThread::on_openFile(const QString &path)
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
    ((MainWindow *)this->parent())->m_totalTime = m_totalTime;

    if (m_totalTime <= 0) {
        qDebug() << "File length error";
        m_pTGFile->close();
    }
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
    } else if (m_pTGFile->atEnd()) {
        m_pTGFile->seek(0);
        m_pts = 0;
    }

    m_stop = false;

    processFrame();
}

void DrawThread::on_stopPlay()
{
    m_stop = true;
}

void DrawThread::on_jumpTo(const quint64 time)
{
    if (!m_pTGFile->seek(time)) {
        qDebug() << "Jump error, may be the end";
        return ;
    }

    FrameInfo frameInfo;
    m_readSize = m_pTGFile->readFrame(m_pBuffer, BUFFER_SIZE, frameInfo);
    m_pts = frameInfo.timestamp;
}

void DrawThread::on_timeout()
{
    long yuvSize, type;
    if (ERR_SUCCESS != PlayMedia_DecodecVideo(m_decHandle, m_pBuffer, m_readSize, m_pYuv, (long *)&yuvSize, (long *)&type)) {
        qDebug() << "Decode Error";
    }

    processFrame();
}

///
/// \brief DrawThread::processFrame
/// include:
/// 1. draw
/// 2. read frame
/// 3. update pts
/// 4. start timer
///
inline void DrawThread::processFrame()
{
    PlayMedia_DDraw(m_drawHandle, m_pYuv, m_width, m_height);
    int sec = m_pts/1000%60;
    int min = m_pts/1000/60%60;
    QString curTime = QString("%1:%2").arg(min, 2, 10, QChar('0')).arg(sec, 2, 10, QChar('0'));
    emit updateCurTime(curTime);
    emit updateSlider(m_pts*100/m_totalTime);


    if (m_stop || m_pTGFile->atEnd()) return;
    FrameInfo frameInfo;
    m_readSize = m_pTGFile->readFrame(m_pBuffer, BUFFER_SIZE, frameInfo);
    int waitTime = frameInfo.timestamp - m_pts;
    m_pts = frameInfo.timestamp;
    if (waitTime < 0) waitTime = 0;

    QTimer::singleShot(waitTime, this, &DrawThread::on_timeout);
}

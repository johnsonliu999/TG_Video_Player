#include "drawthread.h"

#include "mainwindow.h"
#include "tgfile.h"
#include "PlayMediaSDK.h"

#include <QDebug>


DrawThread::DrawThread(QObject *parent) :
    QThread(parent),
    m_pTGFile(new TGFile),
    m_pBuffer(new quint8[BUFFER_SIZE]),
    m_pYuv(new quint8[YUV_SIZE]),
    m_pts(0)
{
    m_drawHandle = ((MainWindow *)this->parent())->m_drawHandle;
    m_decHandle = PlayMedia_CreateVideoDecodec(0);
}

DrawThread::~DrawThread()
{
    delete m_pTGFile;
    delete m_pBuffer;
    delete m_pYuv;
}

//    if (!m_pTGFile->open(AbstractTGFile::MODE_READ)) {
//        qDebug() << "Open file error";
//        return ;
//    }

//    if (!m_pTGFile->seekFrameBeginning()) {
//        qDebug() << "Seek error";
//        return ;
//    }

//    if (m_pTGFile->getTimeLength()<=0) {
//        qDebug() << "Length error";
//        return ;
//    }

//    const quint64 BUFFER_SIZE = 512 * 1024;
//    const quint64 YUV_SIZE = 1920 * 1080 * 3;
//    quint8 buffer[BUFFER_SIZE] = {};
//    quint8 *yuvBuffer = new quint8[YUV_SIZE];

//    quint64 pts, yuvSize, type, width, height, readSize;
//    FrameInfo frameInfo;
//    quint64 decHandle = PlayMedia_CreateVideoDecodec(0);
//    qDebug() << "decHandle :" << decHandle;
//    quint64 drawHandle = ((MainWindow *)this->parent())->m_drawHandle;

//    readSize = m_pTGFile->readFrame(buffer, BUFFER_SIZE, frameInfo);
//    if ( readSize < 0) {
//        qDebug() << "Read Error";
//        return ;
//    }
//    pts = frameInfo.timestamp;

//    if (ERR_SUCCESS != PlayMedia_DecodecVideo(decHandle, buffer, readSize, yuvBuffer, (long *)&yuvSize, (long *)&type)) {
//        qDebug() << "Decode Error";
//        return ;
//    }

//    PlayMedia_GetPictureSize(decHandle, (long*)&width, (long*)&height);
//    qDebug() << "Width :" << width;
//    qDebug() << "Height :" << height;

//    drawHandle = PlayMedia_InitDDraw(drawHandle, width, height);
//    PlayMedia_DDraw(drawHandle, yuvBuffer, width, height);

//    while (!m_pTGFile->atEnd() && !m_stop) {
//        readSize = m_pTGFile->readFrame(buffer, BUFFER_SIZE, frameInfo);
//        if (readSize < 0) {
//            qDebug() << "ReadFrame error";
//            return ;
//        }

//        if (ERR_SUCCESS != PlayMedia_DecodecVideo(decHandle, buffer, readSize, yuvBuffer, (long*)&yuvSize, (long*)&type)) {
//            qDebug() << "Decode error";
//            return ;
//        }

//        QThread::msleep(frameInfo.timestamp - pts);
//        pts = frameInfo.timestamp;

//        PlayMedia_DDraw(drawHandle, yuvBuffer, width, height);
//    }

void DrawThread::openFile(const QString &path)
{
    if (!m_pTGFile->open(path)) {
        qDebug() << "Parse file failed";
        return ;
    }

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
}

void DrawThread::on_startPlay()
{
    if (!m_pTGFile->isOpen()) {
        qDebug() << "File not open";
        return ;
    }

    if (!m_pTGFile->seek(0)) {
        qDebug() << "Seek error";
        return ;
    }
}

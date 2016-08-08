#include "drawthread.h"

#include "mainwindow.h"
#include "tgfile.h"
#include "PlayMediaSDK.h"

#include <QDebug>


DrawThread::DrawThread(const QString &fileName, QObject *parent) :
    QThread(parent),
    m_stop(false),
    m_pTGFile(new TGFile(fileName))
{

}

DrawThread::~DrawThread()
{
    delete m_pTGFile;
}

void DrawThread::run()
{
    qDebug() << "Enter run()";
    if (!m_pTGFile->open(AbstractTGFile::MODE_READ)) {
        qDebug() << "Open file error";
        return ;
    }

    if (!m_pTGFile->seekFrameBeginning()) {
        qDebug() << "Seek error";
        return ;
    }

    if (m_pTGFile->getTimeLength()<=0) {
        qDebug() << "Length error";
        return ;
    }

    const quint64 BUFFER_SIZE = 512 * 1024;
    const quint64 YUV_SIZE = 1920 * 1080 * 3;
    quint8 buffer[BUFFER_SIZE] = {};
    quint8 *yuvBuffer = new quint8[YUV_SIZE];

    quint64 pts, yuvSize, type, width, height, readSize;
    FrameInfo frameInfo;
    quint64 decHandle = PlayMedia_CreateVideoDecodec(0);
    qDebug() << "decHandle :" << decHandle;
    quint64 drawHandle = ((MainWindow *)this->parent())->m_drawHandle;

    readSize = m_pTGFile->readFrame(buffer, BUFFER_SIZE, frameInfo);
    if ( readSize < 0) {
        qDebug() << "Read Error";
        return ;
    }
    pts = frameInfo.timestamp;

    if (ERR_SUCCESS != PlayMedia_DecodecVideo(decHandle, buffer, readSize, yuvBuffer, (long *)&yuvSize, (long *)&type)) {
        qDebug() << "Decode Error";
        return ;
    }

    PlayMedia_GetPictureSize(decHandle, (long*)&width, (long*)&height);
    qDebug() << "Width :" << width;
    qDebug() << "Height :" << height;

    drawHandle = PlayMedia_InitDDraw(drawHandle, width, height);
    PlayMedia_DDraw(drawHandle, yuvBuffer, width, height);

    while (!m_pTGFile->atEnd() && !m_stop) {
        readSize = m_pTGFile->readFrame(buffer, BUFFER_SIZE, frameInfo);
        if (readSize < 0) {
            qDebug() << "ReadFrame error";
            return ;
        }

        if (ERR_SUCCESS != PlayMedia_DecodecVideo(decHandle, buffer, readSize, yuvBuffer, (long*)&yuvSize, (long*)&type)) {
            qDebug() << "Decode error";
            return ;
        }

        QThread::msleep(frameInfo.timestamp - pts);
        pts = frameInfo.timestamp;

        PlayMedia_DDraw(drawHandle, yuvBuffer, width, height);
    }
}

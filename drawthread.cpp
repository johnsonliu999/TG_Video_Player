#include "drawthread.h"

#include "mainwindow.h"
#include "tgfile.h"
#include "PlayMediaSDK.h"

#include <QDebug>


DrawThread::DrawThread(QObject *parent) :
    QThread(parent)
{

}

void DrawThread::run()
{
    qDebug() << "Enter run()";
    TGFile f("0_005_0_20160728152348_940_2_1.tg");
    if (!f.open(AbstractTGFile::MODE_READ)) {
        qDebug() << "Open file error";
        return ;
    }

    if (!f.seekFrameBeginning()) {
        qDebug() << "Seek error";
        return ;
    }

    if (f.getTimeLength()<=0) {
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

    readSize = f.readFrame(buffer, BUFFER_SIZE, frameInfo);
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

    while (!f.atEnd()) {
        readSize = f.readFrame(buffer, BUFFER_SIZE, frameInfo);
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
    exec();
}

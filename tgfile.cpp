#include "tgfile.h"

#include <QFile>
#include <QDebug>

TGFile::TGFile(const QString &path) :
    m_pFile(new QFile(path)),
    m_fileSize(0),
    m_frameSize(0),
    m_frameNum(0),
    m_startTime(0),
    m_endTime(0)
{

}

TGFile::TGFile() :
    m_pFile(new QFile),
    m_fileSize(0),
    m_frameSize(0),
    m_frameNum(0),
    m_startTime(0),
    m_endTime(0)
{

}

TGFile::~TGFile()
{
    delete m_pFile;
}

bool TGFile::open(const AbstractTGFile::OpenMode &mode)
{
    bool res = false;

    switch (mode) {
    case MODE_READ: {
        res = m_pFile->open(QFile::ReadOnly);
        if (!res) return res;

        m_pFile->read((char*)&m_fileHeader, sizeof(FileHeaderBox));
        m_pFile->read((char*)&m_indexHeader, sizeof(IndexHeaderBox));


        for (int i = 0; i < (int)m_indexHeader.indexCount; i++) {
            FrameIndex index;
            m_pFile->read((char*)&index, sizeof(FrameIndex));
            m_indexes << index;

            if (!m_startTime) {
                m_startTime = index.timestamp;
            }
        }

        break;
    }
    case MODE_WRITE: {
        res = m_pFile->open(QFile::WriteOnly | QFile::Append);
        if (!res) return res;
        qDebug() << "Write Mode not support.";
        break;
    }
    }

    return res;
}

bool TGFile::open(const QString &path)
{
    m_pFile->close();
    m_pFile->setFileName(path);

    if (!m_pFile->open(QFile::ReadOnly)) {
        qDebug() << "Open error :" << m_pFile->errorString();
        return false;
    }

    m_pFile->read((char*)&m_fileHeader, sizeof(FileHeaderBox));
    m_pFile->read((char*)&m_indexHeader, sizeof(IndexHeaderBox));

    FrameIndex index;
    m_pFile->read((char*)&index, sizeof(FrameIndex));
    m_indexes << index;
    m_startTime = index.timestamp;

    for (int i = 0; i < (int)m_indexHeader.indexCount-1; i++) {
        m_pFile->read((char*)&index, sizeof(FrameIndex));
        m_indexes << index;
    }
    return true;
}

bool TGFile::isOpen()
{
    return m_pFile->isOpen();
}

void TGFile::close()
{
    m_pFile->close();
}

quint64 TGFile::getTimeLength()
{
    return m_fileHeader.timeLength;
}

quint64 TGFile::getFileSize()
{
    return m_fileSize;
}

PictureSize TGFile::getPictureSize()
{
    return PictureSize(m_fileHeader.width, m_fileHeader.height);
}

///
/// \brief TGFile::readFrame
/// file pointer should be located at certain frame offset
/// \param buffer
/// \param bufSize
/// \param frameInfo
/// \return
///
qint64 TGFile::readFrame(quint8 *buffer, const quint64 &bufSize, FrameInfo &frameInfo)
{
    // read frame header
    FrameHeaderBox frameHeader;
    quint64 readSize;

    if (m_pFile->read((char*)&frameHeader, sizeof(FrameHeaderBox)) < 0) {
        qDebug() << "Read Error :" << m_pFile->errorString();
        return -1;
    }

    readSize = frameHeader.header.size - sizeof(FrameHeaderBox);
    if (bufSize < readSize) {
        qDebug() << "Frame size error";
        qDebug() << "Buf Size" << bufSize;
        qDebug() << "Read Size" << readSize;
    }

    m_pFile->read((char*)buffer, readSize);

    // fill frame type
    switch (frameHeader.frameType) {
    case TG_VIDEO_I:
        frameInfo.frameType = VIDEO_I;
        m_frameNum = 0;
        break;
    case TG_VIDEO_P:
        frameInfo.frameType = VIDEO_P;
        m_frameNum++;
        break;
    case TG_VIDEO_B:
        frameInfo.frameType = VIDEO_B;
        break;
    case TG_AUDIO:
        frameInfo.frameType = AUDIO;
        break;
    }

    // fill frame encode
    if (frameHeader.frameType == TG_AUDIO) {
        switch (m_fileHeader.audioEncodeType) {
        case TG_AUDIO_AAC:
            frameInfo.encodeType = AUDIO_AAC;
            break;
        case TG_AUDIO_ALAW:
            frameInfo.encodeType = AUDIO_ALAW;
            break;
        case TG_AUDIO_ULAW:
            frameInfo.encodeType = AUDIO_ULAW;
            break;
        default:
            frameInfo.encodeType = AUDIO_AAC;
        }
    } else {
        frameInfo.encodeType = VIDEO_H264;
    }

    // fill frame timestamp
    if (frameHeader.frameType == AUDIO) {
        frameInfo.timestamp = m_frameTime;
    } else {
        frameInfo.timestamp = frameHeader.timestamp - m_startTime;
    }

    // fill frame number
    frameInfo.frameNum = m_frameNum;

    return readSize;
}

inline bool TGFile::atEnd()
{
    return m_pFile->atEnd();
}

bool TGFile::seekFrameBeginning()
{
    if (!m_pFile->seek(m_indexes.at(0).offset)) {
        qDebug() << "Seek Error :" << m_pFile->errorString();
        return false;
    }
    qDebug() << "Seek Frame Beginning :" << m_pFile->pos();
    return true;
}

bool TGFile::seek(const quint64 &relativeTime)
{
    for (int i = 0; i < m_indexes.size(); i++) {
        if (m_indexes.at(i).timestamp - m_startTime >= relativeTime) {
            return m_pFile->seek(m_indexes.at(i).offset);
        }
    }
}

qint64 TGFile::pos()
{
    return m_pFile->pos();
}

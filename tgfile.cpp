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
    qDebug() << "TGFile() :" << path;

}

TGFile::~TGFile()
{
    delete m_pFile;
}

bool TGFile::open(const AbstractTGFile::OpenMode &mode)
{
    qDebug() << "Open";
    bool res = false;

    qDebug() << "m_fileheader :" << &m_fileHeader;

    switch (mode) {
    case MODE_READ: {
        res = m_pFile->open(QFile::ReadOnly);
        if (!res) return res;

        m_fileSize = m_pFile->size();

        IndexHeaderBox indexHeader;
        m_pFile->read((char*)&m_fileHeader, sizeof(FileHeaderBox));
        m_pFile->read((char*)&indexHeader, sizeof(IndexHeaderBox));

        qDebug() << sizeof(FileHeaderBox);
        qDebug() << sizeof(IndexHeaderBox);
        qDebug() << sizeof(BoxHeader);
        qDebug() << sizeof(DateTime);

        qDebug() << "Start Time";
        qDebug() << m_fileHeader.startTime.day;
        qDebug() << m_fileHeader.startTime.hour;
        qDebug() << m_fileHeader.startTime.minute;
        qDebug() << m_fileHeader.startTime.second;
        qDebug() << "End Time";
        qDebug() << m_fileHeader.endTime.day;
        qDebug() << m_fileHeader.endTime.hour;
        qDebug() << m_fileHeader.endTime.minute;
        qDebug() << m_fileHeader.endTime.second;


        for (int i = 0; i < (int)indexHeader.indexCount; i++) {
            FrameIndex index;
            m_pFile->read((char*)&index, sizeof(FrameIndex));

            if (!m_startTime) {
                m_startTime = index.timestamp;
            }
        }

        m_pFile->close();

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

void TGFile::close()
{

}

quint64 TGFile::getTimeLength()
{
    return 0;
}

quint64 TGFile::getFileSize()
{
    return 0;
}

PictureSize TGFile::getPictureSize()
{
    return PictureSize();
}

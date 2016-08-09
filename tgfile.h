#ifndef TGFILE_H
#define TGFILE_H

#include "abstracttgfile.h"

#include "tgfiledef.h"

class TGFile : public AbstractTGFile
{
public:
    enum TGFrameType {
        TG_VIDEO_I = 0x01,
        TG_VIDEO_P,
        TG_VIDEO_B,
        TG_AUDIO
    };

    enum TGEncodeType {
        TG_VIDEO_H264 = 0x01,
        TG_AUDIO_AAC = 0X21,
        TG_AUDIO_ALAW,
        TG_AUDIO_ULAW,
        TG_AUDIO_PCM
    };

    TGFile(const QString& path);
    TGFile();
    ~TGFile();

    bool open(const OpenMode &mode) override final;
    bool open(const QString &path);
    bool isOpen();
    void close() override final;
    quint64 getTimeLength() override final;
    quint64 getFileSize() override final;
    PictureSize getPictureSize() override final;
    qint64 readFrame(quint8* buffer, const quint64 &bufSize, FrameInfo &frameInfo) override final;
    bool atEnd() override final;
    bool seek(const quint64 &relativeTime);
    qint64 pos();
private:
    QFile *m_pFile;
    quint64 m_startTime;
    quint64 m_endTime;
    quint64 m_fileSize;
    quint64 m_frameSize;
    quint64 m_frameNum;
    quint64 m_frameTime;
    FileHeaderBox m_fileHeader;
    IndexHeaderBox m_indexHeader;

    QList<FrameIndex> m_indexes;
};

#endif // TGFILE_H

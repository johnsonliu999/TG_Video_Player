#ifndef ABSTRACTTGFILE_H
#define ABSTRACTTGFILE_H

#include <QtGlobal>
#include <QList>

class QFile;
struct PictureSize;
struct FrameInfo;

class AbstractTGFile
{
public:
    enum OpenMode {
        MODE_READ,
        MODE_WRITE
    };

    enum FrameType {
        SPS,
        PPS,
        VIDEO_I,
        VIDEO_P,
        VIDEO_B,
        AUDIO
    };

    enum EncodeType {
        VIDEO_H264,

        AUDIO_AAC,
        AUDIO_ALAW,
        AUDIO_ULAW
    };

    AbstractTGFile();

    virtual bool open(const OpenMode &mode) = 0;
    virtual void close() = 0;
    virtual quint64 getTimeLength() = 0;
    virtual quint64 getFileSize() = 0;
    virtual PictureSize getPictureSize() = 0;
    virtual qint64 readFrame(quint8* buffer, const quint64 &bufSize, FrameInfo &frameInfo) = 0;
    virtual bool atEnd() = 0;
};

struct FrameInfo {
    AbstractTGFile::FrameType frameType;
    AbstractTGFile::EncodeType encodeType;
    quint64 timestamp;
    quint64 frameNum;
};

#endif // ABSTRACTTGFILE_H

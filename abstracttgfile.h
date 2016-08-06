#ifndef ABSTRACTTGFILE_H
#define ABSTRACTTGFILE_H

#include <QtGlobal>
#include <QList>

class QFile;


struct PictureSize {
    quint64 width;
    quint64 height;
};

class AbstractTGFile
{
public:
    enum OpenMode {
        MODE_READ,
        MODE_WRITE
    };

    enum FrameType {
        FRAME_TYPE_SPS,
        FRAME_TYPE_PPS,
        FRAME_TYPE_I,
        FRAME_TYPE_P,
        FRAME_TYPE_B,
        FRAME_TYPE_AUDIO
    };

    enum EncodeType {
        ENCODE_VIDEO_H264,
        ENCODE_AUDIO_AAC,
        ENCODE_AUDIO_ALAW,
        ENCODE_AUDIO_ULAW,
        ENCODE_AUDIO_PCM
    };

    AbstractTGFile();

    virtual bool open(const OpenMode &mode) = 0;
    virtual void close() = 0;
    virtual quint64 getTimeLength() = 0;
    virtual quint64 getFileSize() = 0;
    virtual PictureSize getPictureSize() = 0;
};

#endif // ABSTRACTTGFILE_H

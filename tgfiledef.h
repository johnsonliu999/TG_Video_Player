#ifndef TGFILEDEF_H
#define TGFILEDEF_H

#include <QDebug>

struct DateTime {
    quint16 year;
    quint8 month;
    quint8 day;
    quint8 hour;
    quint8 minute;
    quint8 second;
    quint8 millsec;
};

struct FrameInfo {
    AbstractTGFile::FrameType frameType;
    AbstractTGFile::EncodeType encodeType;
    quint64 timestamp;
    quint64 frameNum;
};

struct FrameIndex {
    quint64 timestamp;
    quint64 offset;
};

struct BoxHeader {
    quint32 nal;
    quint32 flag:8;
    quint32 version:24;
    quint32 boxSize;

    BoxHeader() {
        nal = 0x01000000;
        version = 0x000001;
    }
};

struct IndexHeaderBox {
    BoxHeader header;
    quint32 indexCount;
    quint32 nextIndexHeadOffset;

    IndexHeaderBox() {
        memset((char*)this+sizeof(BoxHeader), 0xff, sizeof(IndexHeaderBox)-sizeof(BoxHeader));
        header.flag = 0x77;
    }
};

struct FrameHeaderBox {
    BoxHeader header;
    quint32 frameType;
    quint32 timestamp;
    quint32 reserve[3];

    FrameHeaderBox() {
        memset((char*)this+sizeof(BoxHeader), 0xff, sizeof(FrameHeaderBox)-sizeof(BoxHeader));
        header.flag = 0x78;
    }
};

struct FileHeaderBox {
    BoxHeader header;

    quint16 videoEncodeType;
    quint16 audioEncodeType;

    quint16 width;
    quint16 height;

    DateTime startTime;
    DateTime endTime;
    quint32 timeLength;
    quint32 frameCount;

    quint32 indexOffset;
    quint32 reserve[20];

    FileHeaderBox() {
        memset((char*)this+sizeof(BoxHeader), 0xff, sizeof(FileHeaderBox)-sizeof(BoxHeader));
        header.flag = 0x79;
    }
};

#endif // TGFILEDEF_H

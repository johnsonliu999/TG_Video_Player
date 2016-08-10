#ifndef TGFILEDEF_H
#define TGFILEDEF_H

#include <QDebug>

#include "abstracttgfile.h"

struct PictureSize {
    quint64 width;
    quint64 height;

    PictureSize(const quint64 &w, const quint64 &h) :
        width(w),height(h)
    {

    }
};

struct DateTime {
    quint16 year;
    quint8 month;
    quint8 day;
    quint8 hour;
    quint8 minute;
    quint8 second;
    quint8 millsec;
};

///
/// \brief The FrameIndex struct
/// Contain frame pts and its offset in the file.
///
struct FrameIndex {
    quint32 timestamp;
    quint32 offset;
};

///
/// \brief The BoxHeader struct
/// basic header information. Every header should contain it.
/// Include version, flag, size.
///
struct BoxHeader {
    quint32 nal;
    quint32 flag:8;
    quint32 version:24;
    quint32 size;

    BoxHeader() {
        nal = 0x01000000;
        version = 0x000001;
    }
};

///
/// \brief The IndexHeaderBox struct
/// index header. Contain index information. Followed by a series of frame indexes.
///
struct IndexHeaderBox {
    BoxHeader header;
    quint32 indexCount;
    quint32 nextIndexHeadOffset;

    IndexHeaderBox() {
        memset((char*)this+sizeof(BoxHeader), 0xff, sizeof(IndexHeaderBox)-sizeof(BoxHeader));
        header.flag = 0x77;
    }
};

///
/// \brief The FrameHeaderBox struct
/// frame header. Contain frame information.
///
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

///
/// \brief The FileHeaderBox struct
/// file header. Contain whole file information.
///
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

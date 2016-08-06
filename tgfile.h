#ifndef TGFILE_H
#define TGFILE_H

#include "abstracttgfile.h"

#include "tgfiledef.h"

class TGFile : public AbstractTGFile
{
public:
    TGFile(const QString& path);
    ~TGFile();

    bool open(const OpenMode &mode) override final;
    void close() override final;
    quint64 getTimeLength() override final;
    quint64 getFileSize() override final;
    PictureSize getPictureSize() override final;

private:
    QFile *m_pFile;
    quint64 m_startTime;
    quint64 m_endTime;
    quint64 m_fileSize;
    quint64 m_frameSize;
    quint64 m_frameNum;
    FileHeaderBox m_fileHeader;

    QList<FrameIndex> m_indexes;
};

#endif // TGFILE_H

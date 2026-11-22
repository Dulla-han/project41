#ifndef VIDEOSELECTION_H
#define VIDEOSELECTION_H

#include <QString>
#include <QtGlobal>

#include "videoreader.h"
#include <QVector>

//describes the selected part of a source video
struct VideoSelection {
    QString path;
    qint64 startFrameIndex = -1;
    qint64 endFrameIndex = -1;
    int keepEvery = 1;
};

struct VideoSequence {
    QVector<VideoFrame> originals;
    QVector<qint64> inputPositions;
    QString error;
};

VideoSequence loadVideoSequence(const VideoSelection &selection);

#endif // VIDEOSELECTION_H

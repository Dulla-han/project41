#ifndef VIDEOREADER_H
#define VIDEOREADER_H

#include <QString>
#include <QtGlobal>

struct AVFormatContext;

struct VideoInfo {
    int width = 0;
    int height = 0;

    double averageFps = 0.0;
    qint64 frameCount = 0;
    double durationSeconds = -1.0;
};

class VideoReader
{
public:
    VideoReader() = default;
    ~VideoReader();

    VideoReader(const VideoReader &) = delete;
    VideoReader &operator(const VideoReader &) = delete;

    bool open(const QString &path);
    void close();

    const VideoInfo &info() const;
    QString errorString() const;

private:
    AVFormatContext *formatContext = nullptr;
    int videoStreamIndex = -1;

    VideoInfo videoInfo;
    QString lastError;
};

#endif // VIDEOREADER_H

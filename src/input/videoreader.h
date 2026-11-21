#ifndef VIDEOREADER_H
#define VIDEOREADER_H

#include <QString>
#include <QtGlobal>
#include <QImage>


//these structs are already defined in FFmpeg lib
//we only need to declare it
//manages video info
struct AVFormatContext;
struct AVCodecContext;
struct AVPacket;
struct AVFrame;
struct SwsContext;

struct VideoInfo {
    int width = 0;
    int height = 0;

    double averageFps = 0.0;
    qint64 frameCount = 0;
    double durationSeconds = -1.0;
};

struct VideoFrame {
    QImage image;
    qint64 index = -1;
    double timeSeconds = -1.0;
};

class VideoReader
{
public:
    VideoReader() = default;
    ~VideoReader();

    VideoReader(const VideoReader &) = delete;
    VideoReader &operator=(const VideoReader &) = delete;

    bool open(const QString &path);
    void close();
    bool readNextFrame(VideoFrame &frame);

    const VideoInfo &info() const;
    QString errorString() const;

private:
    AVFormatContext *formatContext = nullptr;
    //manages video decoder
    AVCodecContext *codecContext = nullptr;
    //contains compressed data of a file
    AVPacket *packet = nullptr;
    //contains decoded frame
    AVFrame *decodedFrame = nullptr;
    //formats image into a preferable format for QImage
    SwsContext *scaleContext = nullptr;

    bool inputEnded = false;

    qint64 nextFrameIndex = 0;
    qint64 firstTimestamp = 0;
    bool hasFirstTimestamp = false;

    int videoStreamIndex = -1;

    VideoInfo videoInfo;
    QString lastError;
};

#endif // VIDEOREADER_H

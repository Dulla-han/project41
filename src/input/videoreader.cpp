#include "videoreader.h"

extern "C" {
#include <libavformat/avformat.h>
#include <libavutil/error.h>
}

namespace {

QString describeError(int errorCode) {
    char buffer[AV_ERROR_MAX_STRING_SIZE] = {};
    av_strerror(errorCode, buffer, sizeof(buffer));
    return QString::fromUtf8(buffer);
}
}

VideoReader::~VideoReader(){
    close();
}

bool VideoReader::open(const QString &path) {
    close();
    lastError.clear();

    const QByteArray encodedPath = path.toUtf8();

    int result = avformat_open_input(&formatContext, encodedPath.constData(), nullptr, nullptr);

    if (result < 0) {
        lastError = "Unable to open video: " + describeError(result);
        close();
        return false;
    }

    result = avformat_find_stream_info(formatContext, nullptr);

    if (result < 0) {
        lastError = "Unable to read stream information: " + describeError(result);
        close();
        return false;
    }

    result = av_find_best_stream(
        formatContext, AVMEDIA_TYPE_VIDEO,
        -1, -1, nullptr, 0
    );

    if (result < 0 ) {
        lastError = "Unable to find a supported video stream: " + describeError(result);
        close();
        return false;
    }

    videoStreamIndex = result;

    const AVStream *stream = formatContext->streams[videoStreamIndex];

    videoInfo.width = stream->codecpar->width;
    videoInfo.height = stream->codecpar->height;
    videoInfo.frameCount= stream->nb_frames;


    const AVRational fps = stream->avg_frame_rate;

    if(fps.num > 0 && fps.den > 0) {
        videoInfo.averageFps = av_q2d(fps);
    }

    if (stream->duration != AV_NOPTS_VALUE) {
        videoInfo.durationSeconds = stream->duration * av_q2d(stream->time_base);
    } else if (formatContext->duration != AV_NOPTS_VALUE) {
        videoInfo.durationSeconds = static_cast<double>(formatContext->duration) / AV_TIME_BASE;
    }

    return true;
}

void VideoReader::close(){
    if (formatContext) {
        avformat_close_input(&formatContext);
    }

    videoStreamIndex = -1;
    videoInfo = VideoInfo();
}

const VideoInfo &VideoReader::info() const {
    return videoInfo;
}

QString VideoReader::errorString() const {
    return lastError;
}
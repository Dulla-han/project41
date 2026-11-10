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

    result = avformat_find_stream_info(formatContext, nulltptr);

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
    videoInfo.frameCount= stream->codecpar->nb_frames;



}

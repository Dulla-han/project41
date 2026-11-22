#include "videoreader.h"

extern "C" {
#include <libavformat/avformat.h>
#include <libavutil/error.h>
#include <libavcodec/avcodec.h>
#include <libavutil/error.h>
#include <libswscale/swscale.h>
}

#include <cerrno>
#include <algorithm>
#include <cmath>

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

    //finds suitable decoder
    const AVCodec *decoder = avcodec_find_decoder(stream->codecpar->codec_id);

    if (!decoder) {
        lastError = "No decoder available for this video.";
        close();
        return false;
    }

    codecContext = avcodec_alloc_context3(decoder);

    if (!codecContext) {
        lastError = "Unable to allocate the video decoder.";
        close();
        return false;
    }

    result = avcodec_parameters_to_context(
        codecContext,
        stream->codecpar
    );

    if (result < 0) {
        lastError = "Unable to copy codec parameters: "
                    + describeError(result);
        close();
        return false;
    }

    codecContext->pkt_timebase = stream->time_base;

    result = avcodec_open2(codecContext, decoder, nullptr);

    if (result < 0) {
        lastError = "Unable to open the video decoder: "
                    + describeError(result);
        close();
        return false;
    }

    packet = av_packet_alloc();
    decodedFrame = av_frame_alloc();

    if (!packet || !decodedFrame) {
        lastError = "Unable to allocate decoding buffers.";
        close();
        return false;
    }

    return true;
}

void VideoReader::close(){
    sws_freeContext(scaleContext);
    scaleContext = nullptr;

    av_frame_free(&decodedFrame);
    av_packet_free(&packet);
    avcodec_free_context(&codecContext);
    avformat_close_input(&formatContext);

    videoStreamIndex = -1;
    nextFrameIndex = 0;
    firstTimestamp = 0;
    hasFirstTimestamp = false;
    videoInfo = VideoInfo();
    inputEnded = false;

    indexedFrameTimes.clear();
}

bool VideoReader::readNextFrame(VideoFrame &frame, bool loadImage) {
    frame = VideoFrame{};
    lastError.clear();

    if (!codecContext || !packet || !decodedFrame) {
        lastError = "No video is open.";
        return false;
    }

    while (true) {
        int result = avcodec_receive_frame(
            codecContext,
            decodedFrame
            );

        if (result == 0) {
            const int width = decodedFrame->width;
            const int height = decodedFrame->height;

            double timeSeconds = -1.0;

            const qint64 timestamp = decodedFrame->best_effort_timestamp;

            if (timestamp != AV_NOPTS_VALUE) {
                if (!hasFirstTimestamp) {
                    firstTimestamp = timestamp;
                    hasFirstTimestamp = true;
                }

                const AVStream *stream = formatContext->streams[videoStreamIndex];

                timeSeconds = (static_cast<double>(timestamp)
                               - static_cast<double>(firstTimestamp))
                              * av_q2d(stream->time_base);
            }

            if (!indexedFrameTimes.isEmpty()) {
                const auto position = std::lower_bound(
                    indexedFrameTimes.cbegin(),
                    indexedFrameTimes.cend(),
                    timeSeconds);

                if (position == indexedFrameTimes.cend() ||
                    *position != timeSeconds) {
                    lastError = "The decoded frame could not be matched to the frame index.";

                    av_frame_unref(decodedFrame);
                    return false;
                }

                nextFrameIndex = position - indexedFrameTimes.cbegin();
            }

            //index building needs frame timing
            if (!loadImage) {
                frame.index = nextFrameIndex;
                frame.timeSeconds = timeSeconds;

                ++nextFrameIndex;
                //release the decoded frame after copying the data
                av_frame_unref(decodedFrame);
                return true;
            }

            scaleContext = sws_getCachedContext(
                scaleContext,
                width,
                height,
                static_cast<AVPixelFormat>(decodedFrame->format),
                width,
                height,
                AV_PIX_FMT_RGB24,
                SWS_BILINEAR,
                nullptr,
                nullptr,
                nullptr
                );

            if (!scaleContext) {
                lastError = "Unable to prepare image conversion.";
                av_frame_unref(decodedFrame);
                return false;
            }

            QImage converted (
                width,
                height,
                QImage::Format_RGB888
                );

            if (converted.isNull()) {
                lastError = "Unable to allocate the output image.";
                av_frame_unref(decodedFrame);
                return false;
            }

            uint8_t *destinationData[4] = {
                converted.bits(), nullptr, nullptr, nullptr
            };

            int destinationStride[4] = {
                static_cast<int>(converted.bytesPerLine()),
                0,0,0
            };

            result = sws_scale(
                scaleContext,
                decodedFrame->data,
                decodedFrame->linesize,
                0,
                height,
                destinationData,
                destinationStride
                );

            av_frame_unref(decodedFrame);

            if (result != height) {
                lastError = "Unable to convert the video frame.";
                return false;
            }

            frame.image = converted;
            frame.index = nextFrameIndex;
            frame.timeSeconds = timeSeconds;

            ++nextFrameIndex;

            return true;
        }

        if (result == AVERROR_EOF) {
            return false;
        }

        if (result != AVERROR(EAGAIN)) {
            lastError = "Unable to decode a frame: "
                        + describeError(result);
            return false;
        }

        if (inputEnded) {
            lastError = "The decoder requested data after end of input.";
            return false;
        }
        //unable to save the save loaded frame into image argument
        //find the next packet belonging to our video stream
        while(true) {
            result = av_read_frame(formatContext, packet);

            if (result < 0) {
                break;
            }

            if (packet->stream_index == videoStreamIndex) {
                break;
            }

            av_packet_unref(packet);
        }

        if (result == AVERROR_EOF) {
            inputEnded = true;
            result = avcodec_send_packet(codecContext, nullptr);
        } else if (result < 0) {
            lastError = "Unable to read video data: "
                        + describeError(result);
            return false;
        } else {
            result = avcodec_send_packet(codecContext, packet);
            av_packet_unref(packet);
        }

        if (result < 0) {
            lastError = "Unable to send data to the decoder: "
                        + describeError(result);
            return false;
        }
    }
}

bool VideoReader::buildFrameIndex(const QString &path) {
    lastError.clear();

    VideoReader scanner;

    if (!scanner.open(path)) {
        lastError = scanner.errorString();
        return false;
    }

    QVector<double> times;
    VideoFrame frame;

    while (scanner.readNextFrame(frame, false)) {
        if (frame.timeSeconds < 0.0) {
            lastError = "Time navigation requires timestamps for every frame.";
            return false;
        }

        if (!times.isEmpty() && frame.timeSeconds <= times.last()) {
            lastError = "Time navigation is not increasing which it should be.";
            return false;
        }

        times.append(frame.timeSeconds);
    }

    if (!scanner.errorString().isEmpty()) {
        lastError = scanner.errorString();
        return false;
    }

    if (times.isEmpty()) {
        lastError = "The video contains no decodable frames.";
        return false;
    }

    indexedFrameTimes = times;

    //keep the same timestamp origin as the indexing reader
    firstTimestamp = scanner.firstTimestamp;
    hasFirstTimestamp = scanner.hasFirstTimestamp;

    return true;
}

bool VideoReader::readFrameAtTime(double seconds, VideoFrame &frame) {
    frame = VideoFrame{};
    lastError.clear();

    if (indexedFrameTimes.isEmpty()) {
        lastError = "The video frame index is not ready.";
        return false;
    }


    auto target = std::lower_bound(
        indexedFrameTimes.cbegin(),
        indexedFrameTimes.cend(),
        std::max(0.0, seconds));

    if (target == indexedFrameTimes.cend()) {
        --target;
    }

    const qint64 targetIndex = target - indexedFrameTimes.cbegin();

    const AVStream *stream = formatContext->streams[videoStreamIndex];

    const qint64 timestamp = firstTimestamp + static_cast<qint64>(std::llround(*target / av_q2d(stream->time_base)));

    const int result = av_seek_frame(
        formatContext,
        videoStreamIndex,
        timestamp,
        AVSEEK_FLAG_BACKWARD);

    if (result < 0) {
        lastError = "Unable to seek in the video: " + describeError(result);
        return false;
    }

    avcodec_flush_buffers(codecContext);
    av_packet_unref(packet);
    av_frame_unref(decodedFrame);
    inputEnded = false;

    while (readNextFrame(frame)) {
        if (frame.index == targetIndex) {
            return true;
        }

        if (frame.index > targetIndex) {
            lastError = "Seeking skipped the requested frame.";
            frame = VideoFrame{};
            return false;
        }
    }

    if (lastError.isEmpty()) {
        lastError = "The requested frame could not be decoded.";
    }

    frame = VideoFrame{};
    return false;
}

const QVector<double> &VideoReader::frameTimes() const {
    return indexedFrameTimes;
}

const VideoInfo &VideoReader::info() const {
    return videoInfo;
}

QString VideoReader::errorString() const {
    return lastError;
}
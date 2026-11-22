#include "videoselection.h"

VideoSequence loadVideoSequence(const VideoSelection &selection) {
    VideoSequence sequence;
    VideoReader reader;

    if (selection.path.isEmpty() ||
        selection.endFrameIndex < 0 ||
        selection.endFrameIndex <= selection.startFrameIndex ||
        selection.keepEvery < 1) {
        sequence.error = "Invalid video selection.";
        return sequence;
    }

    if(!reader.open(selection.path)) {
        sequence.error = reader.errorString();
        return sequence;
    }

    constexpr qint64 memoryLimit = 256LL * 1024 * 1024;
    qint64 usedBytes = 0;
    while (true) {
        VideoFrame frame;

        if (!reader.readNextFrame(frame)) {
            sequence.error = reader.errorString().isEmpty()
                ? QString("The video ended before the selected end frame")
                                 : reader.errorString();

            sequence.originals.clear();
            sequence.inputPositions.clear();
            return sequence;
        }


        //frames before the startIndex
        if (frame.index < selection.startFrameIndex) {
            continue;
        }

        const qint64 imageBytes = static_cast<qint64>(frame.image.sizeInBytes());

        if (imageBytes > memoryLimit - usedBytes) {
            sequence.error = "The selected range exceed the image limit."
                             "Please select a shorter range.";

            sequence.originals.clear();
            sequence.inputPositions.clear();
            return sequence;
        }

        usedBytes += imageBytes;

        const qint64 position = static_cast<qint64>(sequence.originals.size());

        sequence.originals.append(frame);

        const qint64 offset = frame.index - selection.startFrameIndex;

        if (offset % selection.keepEvery == 0 ||
            frame.index == selection.endFrameIndex) {
            sequence.inputPositions.append(position);
        }

        if (frame.index == selection.endFrameIndex) {
            break;
        }
    }

    return sequence;
}
#include "blendinterpolator.h"

#include <algorithm>

QImage interpolateBlend(const QImage &start, const QImage &end, double t) {
    if (start.isNull() || end.isNull() || start.size() != end.size()) {
        return QImage();
    }

    t = std::clamp(t, 0.0, 1.0);

    const QImage first = start.convertToFormat(QImage::Format_RGB32);
    const QImage second = end.convertToFormat(QImage::Format_RGB32);

    QImage result(first.size(), QImage::Format_RGB32);

    for (int y = 0; y < result.height(); ++y) {
        const auto *firstRow = reinterpret_cast<const QRgb *>(first.constScanLine(y));
        const auto *secondRow = reinterpret_cast<const QRgb *>(second.constScanLine(y));
        auto *resultRow = reinterpret_cast<QRgb *>(result.scanLine(y));

        for (int x = 0; x < result.width(); ++x) {
            const QRgb a = firstRow[x];
            const QRgb b = secondRow[x];
            const int red = static_cast<int> ((1.0 - t) * qRed(a) + t * qRed(b) + 0.5);
            const int green = static_cast<int> ((1.0 - t) * qGreen(a) + t * qGreen(b) + 0.5);
            const int blue = static_cast<int> ((1.0 - t) * qBlue(a) + t * qBlue(b) + 0.5);

            resultRow[x] = qRgb(red, green, blue);
        }
    }

    return result;
}
#ifndef BLENDINTERPOLATOR_H
#define BLENDINTERPOLATOR_H

#include <QImage>

QImage interpolateBlend(const QImage &start, const QImage &end, double t);

#endif // BLENDINTERPOLATOR_H

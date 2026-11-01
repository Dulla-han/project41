#include "frameview.h"

#include <QPainter>

FrameView::FrameView(QWidget *parent) : QLabel(parent) {
    setMinimumSize(120, 90);
}

void FrameView::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    int frameWidth = width();
    int frameHeight = frameWidth * 9 / 16;

    if (frameHeight > height()) {
        frameHeight = height();
        frameWidth = frameHeight * 16 / 9;
    }

    const QRect frameRect(
        (width() - frameWidth) / 2,
        (height() - frameHeight) / 2,
        frameWidth,
        frameHeight);

    painter.fillRect(frameRect, Qt::black);

    if (image.isNull()) {
        painter.setPen(Qt::white);
        painter.drawText(frameRect, Qt::AlignCenter, text());
    } else {
        const QSize imageSize = image.size().scaled(frameRect.size(), Qt::KeepAspectRatio);

        const QRect imageRect(
            frameRect.x() + (frameRect.width() - imageSize.width()) / 2,
            frameRect.y() + (frameRect.height() - imageSize.height()) / 2,
            imageSize.width(),
            imageSize.height());

        painter.drawImage(imageRect, image);
    }

    painter.setPen(Qt::gray);
    painter.drawRect(frameRect.adjusted(0,0,-1,-1));
}

void FrameView::setImage(const QImage &newImage)
{
    image = newImage;
    update();
}
#ifndef VIDEOINPUTDIALOG_H
#define VIDEOINPUTDIALOG_H

#include <QDialog>

#include "input/videoreader.h"

namespace Ui {
class VideoInputDialog;
}

class VideoInputDialog : public QDialog
{
    Q_OBJECT

public:
    explicit VideoInputDialog(QWidget *parent = nullptr);
    ~VideoInputDialog();

private:
    Ui::VideoInputDialog *ui;
    VideoReader videoReader;

    void displayFrame(const VideoFrame &frame);

    qint64 startFrameIndex = -1;
    qint64 endFrameIndex = -1;

    void updateSelection();

    VideoFrame currentFrame;
};

#endif // VIDEOINPUTDIALOG_H

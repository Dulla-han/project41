#ifndef VIDEOINPUTDIALOG_H
#define VIDEOINPUTDIALOG_H

#include <QDialog>

#include "input/videoreader.h"
#include "input/videoselection.h"

namespace Ui {
class VideoInputDialog;
}

class VideoInputDialog : public QDialog
{
    Q_OBJECT

public:
    explicit VideoInputDialog(QWidget *parent = nullptr);
    ~VideoInputDialog();
    //returns the selected video path and frame range
    VideoSelection selection() const;

private:
    Ui::VideoInputDialog *ui;
    VideoReader videoReader;

    void displayFrame(const VideoFrame &frame);

    qint64 startFrameIndex = -1;
    qint64 endFrameIndex = -1;

    QString videoPath;

    void updateSelection();

    VideoFrame currentFrame;
};

#endif // VIDEOINPUTDIALOG_H

#include "videoinputdialog.h"
#include "ui_videoinputdialog.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QTime>
#include <cmath>
#include <QSignalBlocker>
#include <algorithm>

VideoInputDialog::VideoInputDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::VideoInputDialog)
{
    ui->setupUi(this);

    ui->nextFrameButton->setEnabled(false);
    ui->keepEverySpinBox->setEnabled(false);
    ui->positionFrameSpinBox->setEnabled(false);
    ui->sourceFrameSlider->setEnabled(false);
    ui->setStartButton->setEnabled(false);
    ui->setEndButton->setEnabled(false);
    ui->useSelectionButton->setEnabled(false);
    ui->positionTimeEdit->setEnabled(false);

    connect(ui->cancelButton, &QPushButton::clicked, this, &QDialog::reject);

    connect(ui->nextFrameButton, &QPushButton::clicked,
            this, [this]() {
            VideoFrame frame;

            if (!videoReader.readNextFrame(frame)) {
                ui->nextFrameButton->setEnabled(false);

                if(!videoReader.errorString().isEmpty()) {
                    QMessageBox::warning(
                        this,
                        "Video Error",
                        videoReader.errorString()
                        );
                } else {
                    ui->nextFrameButton->setToolTip(
                        "End of video reached."
                        );
                }
                return;
                }
            displayFrame(frame);
        }
    );

    connect(ui->setStartButton, &QPushButton::clicked,
            this, [this](){
                if (currentFrame.index < 0) {
                    return;
                }

                startFrameIndex = currentFrame.index;
                ui->startSelectionLabel->setText(
                    QString("Start: %1").arg(startFrameIndex + 1));
                updateSelection();
    });

    connect(ui->setEndButton, &QPushButton::clicked,
            this, [this](){
                if (currentFrame.index < 0) {
                    return;
                }
                endFrameIndex = currentFrame.index;
                ui->endSelectionLabel->setText(
                    QString("End: %1").arg(endFrameIndex + 1));

                updateSelection();
            });

    setWindowTitle("Select Video Range");
    connect(ui->openVideoButton, &QPushButton::clicked,
        this, [this] (){
        const QString path = QFileDialog::getOpenFileName(
        this,
        "Open Video", QString(),
        "Videos (*.mp4 *.mkv *.avi *.mov *.webm);;All Files (*)");

        if (path.isEmpty()) {
            return;
        }

        //reset and ready for the next input video
        ui->sourceFrameView->setImage(QImage{});
        ui->frameNumberLabel->setText("Frame: -");
        ui->nextFrameButton->setEnabled(false);
        currentFrame = VideoFrame{};
        videoPath.clear();

        startFrameIndex = -1;
        endFrameIndex = -1;

        ui->startSelectionLabel->setText("Start: -");
        ui->endSelectionLabel->setText("End: -");

        ui->setStartButton->setEnabled(false);
        ui->setEndButton->setEnabled(false);
        ui->keepEverySpinBox->setEnabled(false);
        ui->positionTimeEdit->setTime(QTime(0, 0, 0));

        ui->sourceFrameSlider->setEnabled(false);
        ui->positionTimeEdit->setEnabled(false);

        updateSelection();

        if(!videoReader.open(path)) {
            ui->videoInfoLabel->setText("No video loaded");

            QMessageBox::warning(
                this,
                "VIDEO ERROR",
                videoReader.errorString());

            return;
        }

        const VideoInfo &info = videoReader.info();

        const QString fpsText = info.averageFps > 0.0
                                    ? QString::number(info.averageFps, 'f', 3)
                                                          : QString("unknown");

        const QString countText = info.frameCount > 0
                                      ? QString::number(info.frameCount)
                                                          : QString("unknown");

        const QString durationText = info.durationSeconds > 0.0
                                         ? QString::number(info.durationSeconds, 'f', 3) + "s"
                                                                    : QString("unknown");

        ui->videoInfoLabel->setText(
            QString("%1 x %2 | Avg. FPS: %3 | Frames: %4 | Duration: %5")
                .arg(info.width)
                .arg(info.height)
                .arg(fpsText)
                .arg(countText)
                .arg(durationText)
        );

        VideoFrame firstFrame;
        if (!videoReader.readNextFrame(firstFrame)) {
            ui->sourceFrameView->setImage(QImage{});

            const QString message = videoReader.errorString().isEmpty()
                ? QString("The video contains no decodable frames.")
                : videoReader.errorString();

            QMessageBox::warning(this, "Video Error", message);
            return;
        }

        //successful first frame load
        videoPath = path;
        displayFrame(firstFrame);


        ui->setStartButton->setEnabled(true);
        ui->setEndButton->setEnabled(true);
        ui->nextFrameButton->setToolTip(QString{});
        ui->nextFrameButton->setEnabled(true);

        if (!videoReader.buildFrameIndex(path)) {
            QMessageBox::warning(this, "Video error", videoReader.errorString());
            return;
        }

        //the slider represents milliseconds
        const double lastTime = videoReader.frameTimes().last();

        const int maximumMilliseconds = static_cast<int>(
            std::min(
                std::ceil(lastTime * 1000.0), 24.0 * 60.0 * 60.0 * 1000.0 - 1));

        {
            const QSignalBlocker blocker(ui->sourceFrameSlider);

            ui->sourceFrameSlider->setRange(0, maximumMilliseconds);
            ui->sourceFrameSlider->setValue(0);
        }

        ui->positionTimeEdit->setTimeRange(
            QTime(0, 0),
            QTime::fromMSecsSinceStartOfDay(maximumMilliseconds));

        ui->sourceFrameSlider->setEnabled(true);
        ui->positionTimeEdit->setEnabled(true);
    });

    connect(ui->useSelectionButton, &QPushButton::clicked,
            this, [this](){
        if (videoPath.isEmpty() || startFrameIndex < 0
            || endFrameIndex <= startFrameIndex) {
            return;
        }

        accept();
    });


    connect(ui->positionTimeEdit, &QTimeEdit::editingFinished,
            this, [this]() {
        showFrameAtTime(ui->positionTimeEdit->time().msecsSinceStartOfDay() / 1000.0);
    });

    ui->sourceFrameSlider->setTracking(false);

    connect(ui->sourceFrameSlider, &QSlider::valueChanged,
            this, [this](int milliseconds) {
        showFrameAtTime(milliseconds / 1000.0);
    });
}

VideoInputDialog::~VideoInputDialog()
{
    delete ui;
}

void VideoInputDialog::displayFrame(const VideoFrame &frame) {
    const QSignalBlocker sliderBlocker(ui->sourceFrameSlider);
    const QSignalBlocker timeBlocker(ui->positionTimeEdit);

    currentFrame = frame;

    ui->sourceFrameView->setImage(frame.image);

    ui->frameNumberLabel->setText(
        QString("Frame: %1").arg(frame.index + 1)
        );

    ui->positionTimeEdit->setTime(QTime(0, 0, 0));
    ui->positionTimeEdit->setToolTip(
        "Frame timestamp unavailable"
        );

    if (frame.timeSeconds < 0.0) {
        return;
    }

    const qint64 milliseconds = static_cast<qint64>(
        std::llround(frame.timeSeconds * 1000.0)
        );

    constexpr qint64 millisecondsPerDay = 24LL * 60 * 60 * 1000;

    if (milliseconds >= millisecondsPerDay) {
        ui->positionTimeEdit->setToolTip(
            "Frame time exceeds the 24-hour display limit."
            );
        return;
    }

    ui->positionTimeEdit->setTime(
        QTime::fromMSecsSinceStartOfDay(
            static_cast<int>(milliseconds)
            )
        );

    ui->sourceFrameSlider->setValue(static_cast<int>(milliseconds));

    ui->positionTimeEdit->setToolTip(
        "Time relative to the first timestamped video frame."
        );
}

void VideoInputDialog::updateSelection() {
    const bool valid = startFrameIndex >= 0
                       && endFrameIndex > startFrameIndex;

    ui->useSelectionButton->setEnabled(valid);
    ui->keepEverySpinBox->setEnabled(valid);
}


VideoSelection VideoInputDialog::selection() const {
    VideoSelection result;

    result.path = videoPath;
    result.startFrameIndex = startFrameIndex;
    result.endFrameIndex = endFrameIndex;
    result.keepEvery = ui->keepEverySpinBox->value();

    return result;
}

void VideoInputDialog::showFrameAtTime(double seconds) {
    VideoFrame frame;

    if (!videoReader.readFrameAtTime(seconds, frame)) {
        QMessageBox::warning(this, "Video error", videoReader.errorString());

        displayFrame(currentFrame);
        return;
    }

    displayFrame(frame);

    //could have been disabled
    ui->nextFrameButton->setEnabled(true);
}

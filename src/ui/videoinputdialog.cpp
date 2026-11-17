#include "videoinputdialog.h"
#include "ui_videoinputdialog.h"

#include <QFileDialog>
#include <QMessageBox>

VideoInputDialog::VideoInputDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::VideoInputDialog)
{
    ui->setupUi(this);

    ui->keepEverySpinBox->setEnabled(false);
    ui->positionFrameSpinBox->setEnabled(false);
    ui->sourceFrameSlider->setEnabled(false);
    ui->setStartButton->setEnabled(false);
    ui->setEndButton->setEnabled(false);
    ui->useSelectionButton->setEnabled(false);
    ui->positionTimeEdit->setEnabled(false);

    connect(ui->cancelButton, &QPushButton::clicked, this, &QDialog::reject);

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

    });
}

VideoInputDialog::~VideoInputDialog()
{
    delete ui;
}

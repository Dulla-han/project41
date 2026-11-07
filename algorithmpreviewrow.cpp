#include "algorithmpreviewrow.h"
#include "ui_algorithmpreviewrow.h"

AlgorithmPreviewRow::AlgorithmPreviewRow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AlgorithmPreviewRow)
{
    ui->setupUi(this);
}

AlgorithmPreviewRow::~AlgorithmPreviewRow()
{
    delete ui;
}

void AlgorithmPreviewRow::setAlgorithmName(const QString &name) {
    ui->algorithmNameLabel->setText(name);
}

void AlgorithmPreviewRow::setFrames(const QVector<QImage> &newFrames) {
    frames = newFrames;
    firstVisibleFrame = 0;
    updateFrames();
}

void AlgorithmPreviewRow::setFirstVisibleFrame(int index) {
    firstVisibleFrame = qMax(0, index);
    updateFrames();
}

void AlgorithmPreviewRow::updateFrames() {
    FrameView *views[] = {
        ui->frameView1,
        ui->frameView2,
        ui->frameView3
    };

    QLabel *captions[] = {
        ui->frame1Label,
        ui->frame2Label,
        ui->frame3Label
    };

    for (int slot = 0; slot < 3; slot++) {
        const int index = firstVisibleFrame + slot;
        FrameView *view = views[slot];
        QLabel *caption = captions[slot];

        if (index >= frames.size()) {
            view->setImage(QImage());
            view->setText("Unknown");
            caption->setText("Unknown");
            continue;
        }

        if (index == 0) {
            caption->setText("Start Frame");
        } else if (index == frames.size() - 1) {
            caption->setText("Ending Frame");
        } else {
            caption->setText(QString("Frame %1").arg(index + 1));
        }

        view->setImage(frames[index]);

        if (frames[index].isNull()) {
            view->setText("Waiting for next step");

        } else {
            view->setText(QString("Frame %1").arg(index + 1));
        }
    }

}
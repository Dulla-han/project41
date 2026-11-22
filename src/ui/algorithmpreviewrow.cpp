#include "algorithmpreviewrow.h"
#include "ui_algorithmpreviewrow.h"

#include <QResizeEvent>
#include <algorithm>

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

void AlgorithmPreviewRow::setFrames(const QVector<PreviewFrame> &newFrames) {
    frames = newFrames;
    firstVisibleFrame = 0;
    updateFrames();
}

void AlgorithmPreviewRow::setFirstVisibleFrame(int index) {
    firstVisibleFrame = qMax(0, index);
    updateFrames();
}

void AlgorithmPreviewRow::updateFrames() {
    //creates an accessible array of UI objects
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
        //accessing the array objects one at a time
        FrameView *view = views[slot];
        QLabel *caption = captions[slot];

        if (index >= frames.size()) {
            view->setImage(QImage());
            view->setText("Unknown");
            caption->setText("Unknown");
            continue;
        }

        const PreviewFrame &item = frames[index];

        view->setImage(item.frame);

        if (item.frame.isNull()) {
            caption->setText("Pending");
            view->setText("Waiting for next step");
            continue;
        }

        if (index == 0) {
            caption->setText("Start Frame");
        } else if (index == frames.size() - 1) {
            caption->setText("Ending Frame");
        } else {
            QString textFrame = item.isOriginal ?
                QString("Original | Frame %1").arg(index + 1) :
                QString("Interpolated | Frame %1").arg(index + 1);
            caption->setText(textFrame);
        }
    }
}

//Resizing the window, the previous function did not work
void AlgorithmPreviewRow::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);

    const auto margins = ui->horizontalLayout->contentsMargins();
    const int spacing = ui->horizontalLayout->spacing();

    //Name of the algorithm has width of 80
    //There are 3 spaces
    const int availableWidth = width() - margins.left() - margins.right() - 80 - 3 * spacing;

    //compute the remaining amount of space
    const int imageWidth = std::max(120, availableWidth / 3);
    //depending on width we compute height to fit the 16:9 ratio
    const int imageHeight = std::max(90, (imageWidth * 9 + 15) / 16);

    const auto columnMargins = ui->verticalLayout->contentsMargins();
    const bool captionVisible = !ui->frame1Label->isHidden();
    const int captionHeight = captionVisible ? ui->frame1Label->height() : 0;
    const int columnSpacing = captionVisible ? ui->verticalLayout->spacing() : 0;

    const int rowHeight = margins.top() + margins.bottom()
    + columnMargins.top() + columnMargins.bottom()
                          + captionHeight + columnSpacing + imageHeight;

    if (height() != rowHeight) {
        setFixedHeight(rowHeight);
    }
}

void AlgorithmPreviewRow::setCaptionVisible(bool visible) {
    ui->frame1Label->setVisible(visible);
    ui->frame2Label->setVisible(visible);
    ui->frame3Label->setVisible(visible);
}
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "blendinterpolator.h"
#include "algorithmpreviewrow.h"
#include "runners/riferunner.h"
#include "videoinputdialog.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QPixmap>
#include <QAction>
#include <QSlider>
#include <algorithm>
#include <QVBoxLayout>
#include <QFutureWatcher>
#include <QtConcurrentRun>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    //makes class able to access the UI objects
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //ALGORITHM
    rifeRunner = new RifeRunner(this);

    connect(rifeRunner, &RifeRunner::started,
            this, [this]() {
                ui->runButton->setEnabled(false);
                ui->actionLoadVideo->setEnabled(false);
                ui->actionLoadFrames->setEnabled(false);
                ui->interpolationMulComboBox->setEnabled(false);
                ui->statusbar->showMessage("RIFE running...");
            });

    connect(rifeRunner, &RifeRunner::finished,
            this, [this](const QVector<QImage> &frames) {
                const int offset = rifePairIndex * runMultiplier;

                for (int step = 1; step < runMultiplier; ++step) {
                    rifePreviewFrames[offset + step] = PreviewFrame{frames[step], false};
                }

                rifeRow->setFrames(rifePreviewFrames);
                updateVisibleFrames();

                ++rifePairIndex;

                if (rifePairIndex + 1 < rifeInputs.size()) {
                    rifeRunner->start(
                        rifeInputs[rifePairIndex],
                        rifeInputs[rifePairIndex + 1],
                        runMultiplier);
                    return;
                }

                ui->runButton->setEnabled(true);
                ui->actionLoadFrames->setEnabled(true);
                ui->actionLoadVideo->setEnabled(true);
                ui->interpolationMulComboBox->setEnabled(true);

                ui->statusbar->showMessage("RIFE finished", 5000);
            });

    connect(rifeRunner, &RifeRunner::failed,
            this, [this](const QString &message) {
                ui->runButton->setEnabled(
                    videoSequence.inputPositions.size() >= 2 ||
                    (!startFrame.frame.isNull() && !endFrame.frame.isNull()));
                ui->actionLoadFrames->setEnabled(true);
                ui->actionLoadVideo->setEnabled(true);
                ui->interpolationMulComboBox->setEnabled(true);
                ui->statusbar->showMessage("RIFE failed", 5000);

                QMessageBox::warning(this, "RIFE", message);
            });

    ui->algorithmPreview1->hide();

    blendRow = new AlgorithmPreviewRow(ui->previewRowsContainer);
    blendRow->setAlgorithmName("Blend");
    blendRow->setMinimumHeight(130);

    auto *rowsLayout = qobject_cast<QVBoxLayout *>(
        ui->previewRowsContainer->layout()
    );

    rifeRow = new AlgorithmPreviewRow(ui->previewRowsContainer);
    rifeRow->setAlgorithmName("RIFE");
    rifeRow->setMinimumHeight(130);

    if (rowsLayout) {
        //inserts a new algorithm row to the beginning of the list
        rowsLayout->insertWidget(0, blendRow);
        blendRow->setCaptionVisible(true);
        rowsLayout->insertWidget(1, rifeRow);
        rifeRow->setCaptionVisible(false);
    }

    //SETUP INTERPOLATION MULTIPLIER COMBOBOX
    ui->interpolationMulComboBox->setItemData(0, 2);
    ui->interpolationMulComboBox->setItemData(1, 4);
    ui->interpolationMulComboBox->setItemData(2, 8);
    ui->interpolationMulComboBox->setItemData(3, 16);

    //SLIDER SETUP
    ui->frameSlider->setRange(0, 0);
    ui->frameSlider->setSingleStep(1);
    ui->frameSlider->setPageStep(1);
    ui->frameSlider->setEnabled(false);

    connect(ui->frameSlider, &QSlider::valueChanged,
            this, [this](int) {
        updateVisibleFrames();
    });

    //BUTTON AND MENU SETUP
    ui->runButton->setEnabled(false);
    //VIDEO DIALOG INPUT SETUP
    ui->actionLoadVideo->setEnabled(true);
    connect(ui->actionLoadVideo, &QAction::triggered,
            this, [this] () {
        if (videoLoading || rifeRunner->isRunning()) {
            return;
        }

        VideoInputDialog dialog(this);

        //cancel leaves the previously stored selection unchanged
        if (dialog.exec() != QDialog::Accepted) {
            return;
        }

        loadVideoSelection(dialog.selection());
    });

    //FRAME SETUP
    //LOAD
    connect(ui->actionLoadFrames, &QAction::triggered,
            this, &MainWindow::loadFrames);

    //PREVIEW PAGE SWITCH
    connect(ui->previewButton, &QPushButton::clicked,
            this, [this](){
        ui->contentStack->setCurrentWidget(ui->previewPage);
    });

    //BENCHMARK PAGE BUTTON
    connect(ui->benchmarkButton, &QPushButton::clicked,
            this, [this](){
        ui->contentStack->setCurrentWidget(ui->benchmarkPage);
    });

    //ALGORITHM RUN SETUP
    connect(ui->runButton, &QPushButton::clicked,
            this, &MainWindow::runExperiment);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//opens 2 dialogs and loads 2 frames
void MainWindow::loadFrames(){
    const QString filter = "Images (*.png *.jpg *.jpeg *.bmp)";
    const QString startPath = QFileDialog::getOpenFileName(this, "Choose the starting frame", QString(), filter);

    if (startPath.isEmpty()){
        return;
    }

    const QString endPath = QFileDialog::getOpenFileName(this, "Choose the ending frame", QString(), filter);

    if (endPath.isEmpty()){
        return;
    }

    PreviewFrame newStart{};
    newStart.frame = QImage(startPath);
    newStart.isOriginal = true;
    PreviewFrame newEnd{};
    newEnd.frame = QImage(endPath);
    newEnd.isOriginal = true;

    if(newStart.frame.isNull() || newEnd.frame.isNull()) {
        QMessageBox::warning(this, "Error", "One of the frames could not be loaded");
        return;
    }


    if(newStart.frame.size() != newEnd.frame.size()) {
        QMessageBox::warning(this, "Error", "Sizes of images does not match");
        return;
    }

    videoSequence = VideoSequence{};
    videoSelection = VideoSelection{};

    startFrame = newStart;
    endFrame = newEnd;

    ui->contentStack->setCurrentWidget(ui->previewPage);

    //adding 3 Images to the previewFrames
    previewFrames = {
        startFrame,
        PreviewFrame{},
        endFrame
    };

    blendRow->setFrames(previewFrames);

    rifeRow->setFrames(previewFrames);

    configurePreviewSlider();

    ui->runButton->setEnabled(true);
}

void MainWindow::updateVisibleFrames() {
    const int index = ui->frameSlider->value();

    blendRow->setFirstVisibleFrame(index);
    rifeRow->setFirstVisibleFrame(index);
}

void MainWindow::runExperiment() {
    if (rifeRunner->isRunning() || videoLoading) return;

    runMultiplier = ui->interpolationMulComboBox->currentData().toInt();

    if (runMultiplier != 2 && runMultiplier != 4 && runMultiplier!= 8 && runMultiplier!= 16 ) {
        return;
    }

    rifeInputs.clear();

    if (!videoSequence.inputPositions.isEmpty()) {
        for (const qint64 position : videoSequence.inputPositions) {
            rifeInputs.append(videoSequence.originals[position].image);
        }
    } else {
        if (startFrame.frame.isNull() || endFrame.frame.isNull()) {
            return;
        }

        rifeInputs.append(startFrame.frame);
        rifeInputs.append(endFrame.frame);
    }

    if (rifeInputs.size() < 2) {
        return;
    }

    previewFrames.clear();
    rifePreviewFrames.clear();
    rifePairIndex = 0;

    for (qsizetype pair = 0; pair + 1 < rifeInputs.size(); ++pair) {
        const QImage &first = rifeInputs[pair];
        const QImage &last = rifeInputs[pair + 1];

        //each interval contributes its starting original
        //and intermediate positions
        //its ending original belongs to the following interval
        //so its not duplicated
        previewFrames.append(PreviewFrame{first, true});
        rifePreviewFrames.append(PreviewFrame{first, true});

        for (int step = 1; step < runMultiplier; ++step) {
            const double t = static_cast<double>(step) / runMultiplier;

            previewFrames.append(PreviewFrame{
                interpolateBlend(first, last, t), false
            });

            rifePreviewFrames.append(PreviewFrame{});
        }
    }

    previewFrames.append(PreviewFrame{rifeInputs.last(), true});
    rifePreviewFrames.append(PreviewFrame{rifeInputs.last(), true});
    blendRow->setFrames(previewFrames);
    rifeRow->setFrames(rifePreviewFrames);

    ui->contentStack->setCurrentWidget(ui->previewPage);
    configurePreviewSlider();

    rifeRunner->start(rifeInputs[0], rifeInputs[1], runMultiplier);
}

void MainWindow::loadVideoSelection(const VideoSelection &selection) {
    if (videoLoading || rifeRunner->isRunning()) {
        return;
    }

    videoLoading = true;

    ui->runButton->setEnabled(false);
    ui->actionLoadFrames->setEnabled(false);
    ui->actionLoadVideo->setEnabled(false);
    ui->statusbar->showMessage("Loading video frames...");

    //watches the result and is automatically destroyed with the window
    auto *watcher = new QFutureWatcher<VideoSequence>(this);

    connect(watcher, &QFutureWatcher<VideoSequence>::finished,
            this, [this, watcher, selection]() {
        const VideoSequence result = watcher->result();
        watcher->deleteLater();

        videoLoading = false;

        ui->actionLoadFrames->setEnabled(true);
        ui->actionLoadVideo->setEnabled(true);

        if (!result.error.isEmpty()) {
            ui->runButton->setEnabled(!startFrame.frame.isNull() &&
                                      !endFrame.frame.isNull());

            ui->statusbar->showMessage("Video loading failed", 5000);

            QMessageBox::warning(this, "Video Error", result.error);

            return;
        }

        videoSelection = selection;
        videoSequence = result;

        startFrame.frame = QImage{};
        startFrame.isOriginal = false;
        endFrame.frame = QImage{};
        endFrame.isOriginal= false;

        showVideoSequence();

        ui->runButton->setEnabled(
            videoSequence.inputPositions.size() >= 2 ||
            (!startFrame.frame.isNull() && !endFrame.frame.isNull()));

        ui->statusbar->showMessage(QString("Loaded %1 original frames | %2 algorithm input frames")
                                       .arg(videoSequence.originals.size())
                                       .arg(videoSequence.inputPositions.size()));
    });

    //capture only selection by value
    //the worker doesn't depend on the main window remaining alive
    watcher->setFuture(QtConcurrent::run([selection](){
        return loadVideoSequence(selection);
        })
    );
}

void MainWindow::showVideoSequence() {
    previewFrames.clear();
    previewFrames.reserve(videoSequence.inputPositions.size());

    for (const qint64 position : videoSequence.inputPositions) {
        previewFrames.append(PreviewFrame{videoSequence.originals[position].image, true});
    }

    blendRow->setFrames(previewFrames);
    rifeRow->setFrames(previewFrames);

    configurePreviewSlider();
}

void MainWindow::configurePreviewSlider() {
    const int maximumIndex = std::max(0, static_cast<int>(previewFrames.size()) - 3);

    ui->frameSlider->setRange(0, maximumIndex);
    ui->frameSlider->setValue(0);
    ui->frameSlider->setEnabled(maximumIndex > 0);

    updateVisibleFrames();
}
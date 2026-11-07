#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "blendinterpolator.h"
#include "algorithmpreviewrow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QPixmap>
#include <QAction>
#include <QSlider>
#include <algorithm>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //ALGORITHM BLEND ROW SETUP
    ui->algorithmPreview1->hide();

    blendRow = new AlgorithmPreviewRow(ui->previewRowsContainer);
    blendRow->setAlgorithmName("Blend");
    blendRow->setMinimumHeight(130);

    auto *rowsLayout = qobject_cast<QVBoxLayout *>(
        ui->previewRowsContainer->layout()
    );

    if (rowsLayout) {
        //inserts a new algorithm row to the beginning of the list
        rowsLayout->insertWidget(0, blendRow);
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

    connect(ui->frameSlider, & QSlider::valueChanged,
            this, [this](int) {
        updateVisibleFrames();
    });

    //BUTTON AND MENU SETUP
    ui->runButton->setEnabled(false);
    ui->actionLoadVideo->setEnabled(false);

    //FRAME SETUP
    connect(ui->actionLoadFrames, &QAction::triggered,
            this, &MainWindow::loadFrames);

    connect(ui->previewButton, &QPushButton::clicked,
            this, [this](){
        ui->contentStack->setCurrentWidget(ui->previewPage);
    });

    connect(ui->benchmarkButton, &QPushButton::clicked,
            this, [this](){
        ui->contentStack->setCurrentWidget(ui->benchmarkPage);
    });

    //EXPERIMENT SETUP
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
        QMessageBox::warning(this, "Error", "Input path is empty or corrupted");
        return;
    }

    const QString endPath = QFileDialog::getOpenFileName(this, "Choose the ending frame", QString(), filter);

    if (endPath.isEmpty()){
        QMessageBox::warning(this, "Error", "Input path is empty or corrupted");
        return;
    }

    const QImage newStart(startPath);
    const QImage newEnd(endPath);

    if(newStart.isNull() || newEnd.isNull()) {
        QMessageBox::warning(this, "Error", "One of the frames could not be loaded");
        return;
    }


    if(newStart.size() != newEnd.size()) {
        QMessageBox::warning(this, "Error", "Sizes of images does not match");
        return;
    }

    startFrame = newStart;
    endFrame = newEnd;

    ui->contentStack->setCurrentWidget(ui->previewPage);


    previewFrames = {
        startFrame,
        QImage(),
        endFrame
    };

    blendRow->setFrames(previewFrames);

    const int maximumIndex = std::max(0, static_cast<int>(previewFrames.size()) - 3);

    ui->frameSlider->setRange(0, maximumIndex);
    ui->frameSlider->setValue(0);
    ui->frameSlider->setEnabled(maximumIndex > 0);

    updateVisibleFrames();

    ui->runButton->setEnabled(true);
}

void MainWindow::updateVisibleFrames() {
    blendRow->setFirstVisibleFrame(
        ui->frameSlider->value()
        );
}

void MainWindow::runExperiment() {
    if (startFrame.isNull() || endFrame.isNull()) return;

    const int multiplier = ui->interpolationMulComboBox->currentData().toInt();

    if (multiplier != 2 && multiplier != 4 && multiplier != 8 && multiplier != 16 ) {
        return;
    }

    const int frameCount = multiplier - 1;

    previewFrames.clear();
    previewFrames.append(startFrame);

    for (int i = 1; i <= frameCount; i++) {
        const double t = static_cast<double>(i) / multiplier;

        previewFrames.append(interpolateBlend(startFrame, endFrame, t));
    }

    previewFrames.append(endFrame);
    blendRow->setFrames(previewFrames);

    const int maximumIndex = std::max(0, static_cast<int>(previewFrames.size()) - 3);

    ui->frameSlider->setRange(0, maximumIndex);
    ui->frameSlider->setValue(0);
    ui->frameSlider->setEnabled(maximumIndex > 0);

    ui->contentStack->setCurrentWidget(ui->previewPage);
    updateVisibleFrames();
}
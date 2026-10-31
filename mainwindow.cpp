#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QPixmap>
#include <QAction>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->runButton->setEnabled(false);
    ui->actionLoadVideo->setEnabled(false);

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
}

MainWindow::~MainWindow()
{
    delete ui;
}


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

    ui->startFrameLabel->setPixmap(
        QPixmap::fromImage(startFrame).scaled(ui->startFrameLabel->contentsRect().size(),
                                              Qt::KeepAspectRatio,
                                              Qt::SmoothTransformation
    ));

    ui->endFrameLabel->setPixmap(
        QPixmap::fromImage(endFrame).scaled(ui->endFrameLabel->contentsRect().size(),
                                              Qt::KeepAspectRatio,
                                              Qt::SmoothTransformation
    ));

    ui->generatedFrameLabel->clear();
    ui->generatedFrameLabel->setText("Waiting run permission...");

    ui->runButton->setEnabled(true);

}
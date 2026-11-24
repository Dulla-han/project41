#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QVector>
#include <QString>

#include "input/videoselection.h"
#include "algorithmpreviewrow.h"

class AlgorithmPreviewRow;
class RifeRunner;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void loadFrames();
    void updateVisibleFrames();
    void runExperiment();
    //the currently loaded input is replaced only after successful loading
    void loadVideoSelection(const VideoSelection &selection);
    //loads the retained original inputs into each row, configures the navigation
    void showVideoSequence();
    //configures scrolling for three visible frames and returs to the start
    void configurePreviewSlider();

    QVector<PreviewFrame> previewFrames;

    PreviewFrame startFrame;
    PreviewFrame endFrame;

    Ui::MainWindow *ui;

    AlgorithmPreviewRow *blendRow = nullptr;

    AlgorithmPreviewRow *rifeRow = nullptr;
    RifeRunner *rifeRunner = nullptr;
    QVector<QImage> rifeInputs;
    QVector<PreviewFrame> rifePreviewFrames;

    int rifePairIndex = 0;

    int runMultiplier = 2;

    VideoSelection videoSelection;
    VideoSequence videoSequence;
    bool videoLoading = false;

    bool algorithmIntepolated = false;

};

#endif // MAINWINDOW_H

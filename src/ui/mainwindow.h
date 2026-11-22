#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QVector>
#include <QString>

#include "input/videoselection.h"

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



    QVector<QImage> previewFrames;

    QImage startFrame;
    QImage endFrame;

    Ui::MainWindow *ui;

    AlgorithmPreviewRow *blendRow = nullptr;

    AlgorithmPreviewRow *rifeRow = nullptr;
    RifeRunner *rifeRunner = nullptr;

    QString startFramePath;
    QString endFramePath;

    VideoSelection videoSelection;
    VideoSequence videoSequence;
    bool videoLoading = false;

};

#endif // MAINWINDOW_H

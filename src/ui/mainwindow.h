#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QVector>
#include <QString>

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

    QVector<QImage> previewFrames;

    QImage startFrame;
    QImage endFrame;

    Ui::MainWindow *ui;

    AlgorithmPreviewRow *blendRow = nullptr;

    AlgorithmPreviewRow *rifeRow = nullptr;
    RifeRunner *rifeRunner = nullptr;

    QString startFramePath;
    QString endFramePath;

};

#endif // MAINWINDOW_H

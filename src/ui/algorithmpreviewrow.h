#ifndef ALGORITHMPREVIEWROW_H
#define ALGORITHMPREVIEWROW_H

#include <QWidget>
#include <QVector>
#include <QString>
#include <QImage>

//stores the image and whether it comes from original input
struct PreviewFrame {
    QImage frame;
    bool isOriginal=false;
};

namespace Ui {
class AlgorithmPreviewRow;
}

class AlgorithmPreviewRow : public QWidget
{
    Q_OBJECT

public:
    explicit AlgorithmPreviewRow(QWidget *parent = nullptr);
    ~AlgorithmPreviewRow();
    void setAlgorithmName(const QString &name);
    void setFrames(const QVector<PreviewFrame> &frames);
    void setFirstVisibleFrame(int index);
    void setCaptionVisible(bool visible);

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    void updateFrames();

    QVector<PreviewFrame> frames;
    int firstVisibleFrame = 0;

    Ui::AlgorithmPreviewRow *ui;
};

#endif // ALGORITHMPREVIEWROW_H

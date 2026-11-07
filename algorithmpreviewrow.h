#ifndef ALGORITHMPREVIEWROW_H
#define ALGORITHMPREVIEWROW_H

#include <QWidget>
#include <QVector>
#include <QString>
#include <QImage>

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
    void setFrames(const QVector<QImage> &frames);
    void setFirstVisibleFrame(int index);

private:
    void updateFrames();

    QVector<QImage> frames;
    int firstVisibleFrame = 0;

    Ui::AlgorithmPreviewRow *ui;
};

#endif // ALGORITHMPREVIEWROW_H

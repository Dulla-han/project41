#ifndef RIFERUNNER_H
#define RIFERUNNER_H

#include <QObject>
#include <QImage>
#include <QString>
#include <QVector>

class RifeRunner : public QObject
{
    Q_OBJECT

public:
    explicit RifeRunner(QObject *parent = nullptr);

    void start(const QImage &startFrame, const QImage &endFrame, int multiplier);

    bool isRunning() const;

signals:
    void started();
    void finished(const QVector<QImage> &frames);
    void failed(const QString &message);

private:
    bool running = false;
};

#endif // RIFERUNNER_H

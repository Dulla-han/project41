#include "riferunner.h"

#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QProcess>
#include <QTemporaryDir>
#include <memory>

RifeRunner::RifeRunner(QObject *parent)
    : QObject(parent)
{
}

bool RifeRunner::isRunning() const
{
    return running;
}

void RifeRunner::start(const QImage &startFrame,
                       const QImage &endFrame,
                       int multiplier)
{
    if (running)
        return;

    if (startFrame.isNull() || endFrame.isNull() ||
        startFrame.size() != endFrame.size()) {
        emit failed("Incorrect inputs of frames");
        return;
    }

    if (multiplier != 2 && multiplier != 4 &&
        multiplier != 8 && multiplier != 16) {
        emit failed("Not supported multiplier of RIFE");
        return;
    }

    QDir projectDir(QCoreApplication::applicationDirPath());

    while (!projectDir.exists("algorithms/RIFE/inference_img.py")) {
        if (!projectDir.cdUp()) {
            emit failed("Unable to find the RIFE script.");
            return;
        }
    }

    const QString pythonPath =
        projectDir.absoluteFilePath(".venv-rife/bin/python");
    const QString scriptPath =
        projectDir.absoluteFilePath("algorithms/RIFE/inference_img.py");
    const QString modelPath =
        projectDir.absoluteFilePath("algorithms/RIFE/train_log");

    if (!QFileInfo::exists(pythonPath)) {
        emit failed("Unable to find Python in .venv-rife");
        return;
    }

    auto workDir = std::make_shared<QTemporaryDir>();

    if (!workDir->isValid()) {
        emit failed("Unable to make a work folder");
        return;
    }

    const QString firstPath = workDir->filePath("start.png");
    const QString lastPath = workDir->filePath("end.png");

    if (!startFrame.convertToFormat(QImage::Format_RGB888).save(firstPath)
        || !endFrame.convertToFormat(QImage::Format_RGB888).save(lastPath)) {
        emit failed("Unable to save the inputs");
        return;
    }

    int exponent = 0;
    for (int value = multiplier; value > 1; value /= 2)
        ++exponent;

    const QSize expectedSize = startFrame.size();

    auto *process = new QProcess(this);
    process->setProcessChannelMode(QProcess::MergedChannels);
    process->setWorkingDirectory(workDir->path());

    auto cleanup = [this, process, workDir]() {
        running = false;
        process->deleteLater();
    };

    connect(process, &QProcess::errorOccurred,
            this, [this, process, cleanup](QProcess::ProcessError error) {
                if (error == QProcess::FailedToStart) {
                    const QString message = process->errorString();
                    cleanup();
                    emit failed(message);
                }
            });

    connect(process, &QProcess::finished,
            this,
            [this, process, workDir, multiplier, expectedSize, cleanup]
            (int exitCode, QProcess::ExitStatus status) {
                if (status != QProcess::NormalExit || exitCode != 0) {
                    const QString log =
                        QString::fromLocal8Bit(process->readAll());

                    cleanup();
                    emit failed("RIFE calculation failed:\n" + log);
                    return;
                }

                QVector<QImage> results;

                for (int i = 0; i <= multiplier; ++i) {
                    const QString path = workDir->filePath(
                        QString("output/img%1.png").arg(i));

                    QImage frame(path);

                    if (frame.isNull() || frame.size() != expectedSize) {
                        cleanup();
                        emit failed("Incorrect output of RIFE:\n" + path);
                        return;
                    }

                    results.append(frame);
                }

                cleanup();
                emit finished(results);
            });

    running = true;
    emit started();

    process->start(pythonPath, QStringList{
                                   scriptPath,
                                   "--img", firstPath, lastPath,
                                   "--exp", QString::number(exponent),
                                   "--model", modelPath
                               });
}

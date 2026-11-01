#ifndef FRAMEVIEW_H
#define FRAMEVIEW_H

#include <QLabel>
#include <QImage>

class FrameView : public QLabel {
public:
    explicit FrameView(QWidget *parent = nullptr);

    void setImage(const QImage &image);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QImage image;
};


#endif // FRAMEVIEW_H


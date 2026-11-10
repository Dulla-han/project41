#include "videoinputdialog.h"
#include "ui_videoinputdialog.h"

VideoInputDialog::VideoInputDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::VideoInputDialog)
{
    ui->setupUi(this);

    ui->keepEverySpinBox->setEnabled(false);
    ui->positionFrameSpinBox->setEnabled(false);
    ui->sourceFrameSlider->setEnabled(false);
    ui->setStartButton->setEnabled(false);
    ui->setEndButton->setEnabled(false);
    ui->useSelectionButton->setEnabled(false);
    ui->positionTimeEdit->setEnabled(false);

    connect(ui->cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

VideoInputDialog::~VideoInputDialog()
{
    delete ui;
}

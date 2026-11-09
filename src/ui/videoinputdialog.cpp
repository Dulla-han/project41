#include "videoinputdialog.h"
#include "ui_videoinputdialog.h"

VideoInputDialog::VideoInputDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::VideoInputDialog)
{
    ui->setupUi(this);
}

VideoInputDialog::~VideoInputDialog()
{
    delete ui;
}

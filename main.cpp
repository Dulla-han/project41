#include <QApplication>
#include "mainwindow.h"


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow window;
    window.showMaximized();
    window.resize(1280, 800);
    window.show();

    return app.exec();
}

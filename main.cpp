#include "mainwindow.h"

#include <QApplication>

int main(int argc, char** argv)
{
    QApplication::setStyle("Fusion");

    QApplication app(argc, argv);

    MainWindow window;
    window.show();

    app.exec();

    return 0;
}


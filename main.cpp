#include "mainwindow.hpp"

#include <QApplication>



int main(int argc, char *argv[])
{
    QApplication::setStyle("fusion");

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}

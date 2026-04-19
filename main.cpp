#include "mainwindow.hpp"
#include "settings.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    Settings::applyTheme(Settings::currentTheme());
    
    MainWindow w;
    w.show();
    return a.exec();
}

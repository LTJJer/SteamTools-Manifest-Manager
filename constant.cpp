#include "constant.hpp"

#include <QFile>
#include <QTextStream>



namespace Constant
{
QString getDarkThemeStyleSheet()
{
    QFile file(":/StyleSheet/DarkThemeStyleSheet.qss");
    if (!file.open(QIODevice::Text | QIODevice::ReadOnly)) return "";

    return QTextStream(&file).readAll();
}

QString getLightThemeStyleSheet()
{
    QFile file(":/StyleSheet/LightThemeStyleSheet.qss");
    if (!file.open(QIODevice::Text | QIODevice::ReadOnly)) return "";

    return QTextStream(&file).readAll();
}



QSettings getSettings()
{
    return QSettings("settings.ini", QSettings::IniFormat);
}
}

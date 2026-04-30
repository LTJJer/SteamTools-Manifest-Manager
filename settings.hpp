#pragma once

#include <QSettings>



class Settings
{
signals:


public:
    Settings();
    Settings(const QString &organization, const QString &application = "");
    ~Settings() = default;

public:


private:
    QSettings mv_settings;
}
settings;

#include "settings.hpp"



Settings::Settings()
    : mv_settings()
{}

Settings::Settings(const QString &organization, const QString &application)
    : mv_settings(organization, application)
{}

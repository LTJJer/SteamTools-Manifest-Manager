#pragma once

#include <QRegularExpression>
#include <QSettings>



namespace Constant
{
namespace Role
{
constexpr int path = Qt::UserRole + 0;
constexpr int name = Qt::UserRole + 1;
constexpr int appid = Qt::UserRole + 2;
}


constexpr char luaEnabledSuffix[]  = "lua";
constexpr char luaDisabledSuffix[] = "disabled";
constexpr char ItemDisabledStyleSheet[] = "color: #FF4444";



inline const QRegularExpression pathPattern(R"pattern([^\\/:*?"<>|]*)pattern");



QString getDarkThemeStyleSheet();
QString getLightThemeStyleSheet();



QSettings getSettings();
}

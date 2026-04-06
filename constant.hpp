#pragma once

#include <QRegularExpression>



namespace Constant
{
constexpr int filePathRole  = Qt::UserRole + 0;
constexpr int gameNameRole  = Qt::UserRole + 1;
constexpr int appidRole = Qt::UserRole + 2;

constexpr char luaEnabledSuffix[]  = "lua";
constexpr char luaDisabledSuffix[] = "disabled";
constexpr char ItemDisabledStyleSheet[] = "color: #FF4444";



inline const QRegularExpression pathRegex(R"pattern([^\\/:*?"<>|]*)pattern");
}

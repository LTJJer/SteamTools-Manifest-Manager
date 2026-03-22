#pragma once

#include <QFlags>
#include <QStringList>
#include <QFile>
#include <qevent.h>
#include <QMimeData>



namespace Constant
{
    constexpr int filePathRole  = Qt::UserRole + 0;
    constexpr int gameNameRole  = Qt::UserRole + 1;
    constexpr int appidRole = Qt::UserRole + 2;

    constexpr char luaEnabledSuffix[]  = "lua";
    constexpr char luaDisabledSuffix[] = "disabled";
    constexpr char ItemDisabledStyleSheet[] = "color: #FF4444";
}



namespace FunctionLib
{
struct LuaInfo
{
    bool hasName = false;
    bool hasAppID = false;

    QString name = "";
    QString appID = "";


    static QStringList toList(const QString &name, const QString &appid);
    static QString toString(const QString &name, const QString &appid);

    QStringList toList();
    QString toString();
};

enum FileEditErrorTypeFlag
{
    NoError = 0,

    NewNameExisted =   0b1,
    RenameFailed   =  0b10,
    OpenFileFailed = 0b100
};
Q_DECLARE_FLAGS(FileEditErrorType, FileEditErrorTypeFlag)

QString generateFileEditErrorString(FileEditErrorType flags);



void closeSteam();
void closeSteamDetached();
void restartSteam(const QString &steamFilePath, int interval = 500);

void steamRunGame(const QString &appid);
void steamOpenShop(const QString &appid);

void steamAppSearch(const QString &content);
void steamWebSearch(const QString &content);
void steamDBSearch(const QString &content);
void steamUISearch(const QString &content);
void bingSearch(const QString &content);
void baiduSearch(const QString &content);



void explorerOpenPath(const QString &path);
void explorerSelectPath(const QString &path);



QStringList splitStringLines(const QString &content, bool skipEmpty = true);

LuaInfo detectLuaInfo(const QStringList &content, const QString &defaultName = "", const QString &defaultAppID = "");
LuaInfo detectLuaInfo(const QString &content, const QString &defaultName = "", const QString &defaultAppID = "");

void formatLua(QStringList &content, const QString &name, const QString &appid);
void formatLua(QString &content, const QString &name, const QString &appid);
void formatLua(QStringList &content);
void formatLua(QString &content);
QString formattedLua(const QString &content, const QString &name, const QString &appid);
QString formattedLua(const QString &content);


FileEditErrorType renameFile(QFile *file, const QString &newName);
FileEditErrorType renameFile(const QString &filePath, const QString &newName);

FileEditErrorType editLuaFile(const QString &filePath, const QString &gameName, const QString &gameAppID, bool shouleRename = true);



QStringList getMimeDataPaths(const QMimeData *mime);
}

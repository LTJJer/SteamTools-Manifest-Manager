#pragma once

#include <QFlags>
#include <QStringList>
#include <QFile>
#include <qevent.h>
#include <QMimeData>
#include "QRegularExpressionValidator"



namespace Constant
{
    constexpr int filePathRole  = Qt::UserRole + 0;
    constexpr int gameNameRole  = Qt::UserRole + 1;
    constexpr int appidRole = Qt::UserRole + 2;

    constexpr char luaEnabledSuffix[]  = "lua";
    constexpr char luaDisabledSuffix[] = "disabled";
    constexpr char ItemDisabledStyleSheet[] = "color: #FF4444";



    inline const QRegularExpression pathRegex(R"pattern([^\\/:*?"<>|]*)pattern");
    inline const QRegularExpressionValidator pathValidator(pathRegex);
}



namespace FunctionLib
{
struct LuaInfo
{
    bool hasName = false;
    bool hasAppid = false;

    QString name = "";
    QString appid = "";


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

template <typename T>
concept StringListJoinAble = requires (T sep)
{
    { std::declval<QStringList>().join(sep) } -> std::convertible_to<QString>;
};

template <StringListJoinAble T = char>
inline QString generateFileEditErrorString(const FileEditErrorType flags, const T join = '\n')
{
    QStringList errorContent;

    if (flags & FunctionLib::NewNameExisted) errorContent.append("文件名更新失败——新文件名已存在");
    if (flags & FunctionLib::RenameFailed)   errorContent.append("文件名更新失败——重命名失败");
    if (flags & FunctionLib::OpenFileFailed) errorContent.append("文件编辑失败——文件打开失败");

    return errorContent.join(join);
}



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



QStringList splitStringLines(const QString &content);

LuaInfo findLuaInfo(const QString &content, const QString &defaultName = "", const QString &defaultAppid = "");
LuaInfo findLuaInfo(const QStringList &content, const QString &defaultName = "", const QString &defaultAppid = "");

QString formattedLua(const QString &content, const QString &name, const QString &appid);
void formatLua(QStringList &content, const QString &name, const QString &appid);



FileEditErrorType renameFile(QFile *file, const QString &newName);
FileEditErrorType renameFile(const QString &filePath, const QString &newName);

FileEditErrorType editLuaFile(const QString &filePath, const QString &gameName, const QString &gameAppid, bool shouldRename = true);



QStringList getMimeDataPaths(const QMimeData *mime);
}

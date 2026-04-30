#pragma once

#include <QFile>
#include <QMimeData>
#include <QWidget>



namespace FunctionLib
{
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

    if      (flags & FunctionLib::NewNameExisted) errorContent.append("文件名更新失败——新文件名已存在");
    else if (flags & FunctionLib::RenameFailed  ) errorContent.append("文件名更新失败——重命名失败");
    if      (flags & FunctionLib::OpenFileFailed) errorContent.append("文件编辑失败——文件打开失败");

    return errorContent.join(join);
}



QString toWindowsPath(QString path);

FileEditErrorType renameFile(QFile *file, const QString &newName);
FileEditErrorType renameFile(const QString &filePath, const QString &newName);



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



QStringList getMimeDataPaths(const QMimeData *mime);



bool isSystemThemeLight();
bool isThemeLight();

QString getSystemThemeStyleSheet();
QString getThemeStyleSheet();

void applyThemeStyle(QWidget *widget);
}

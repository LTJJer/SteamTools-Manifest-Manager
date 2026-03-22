#include "functionlib.hpp"

#include <QProcess>
#include <QTimer>
#include <QRegularExpression>
#include <QFileInfo>
#include <QDesktopServices>
#include <QUrl>
#include <QDir>



namespace FunctionLib
{
QStringList LuaInfo::toList(const QString &name, const QString &appid)
{
    return QStringList({
                        "-- 游戏名称: " + name,
                        "-- AppID: " + appid,
                        ""
    });
}

QString LuaInfo::toString(const QString &name, const QString &appid)
{
    return LuaInfo::toList(name, appid).join("\n");
}

QStringList LuaInfo::toList()
{
    return LuaInfo::toList(this->name, this->appID);
}

QString LuaInfo::toString()
{
    return LuaInfo::toString(this->name, this->appID);
}



QString generateFileEditErrorString(FileEditErrorType flags)
{
    QStringList errorContent;

    if (flags & FunctionLib::NewNameExisted) errorContent.append("文件名更新失败：新文件名已存在");
    if (flags & FunctionLib::RenameFailed)   errorContent.append("文件名更新失败：重命名失败");
    if (flags & FunctionLib::OpenFileFailed) errorContent.append("文件编辑失败：文件打开失败");

    return errorContent.join("\n");
}



void closeSteam()
{
    QProcess process;
    process.start("taskkill", QStringList() << "/f" << "/im" << "Steam.exe");
    process.waitForFinished();
}

void closeSteamDetached()
{
    QProcess::startDetached("taskkill", QStringList() << "/f" << "/im" << "Steam.exe");
}

void restartSteam(const QString &steamFilePath, int interval)
{
    closeSteam();

    QTimer::singleShot(interval, [steamFilePath]
                       {
                           QProcess::startDetached(steamFilePath);
                       });
}


void steamRunGame(const QString &appid)
{
    QDesktopServices::openUrl(QUrl("steam://rungameid/" + appid));
}

void steamOpenShop(const QString &appid)
{
    QDesktopServices::openUrl(QUrl("steam://store/" + appid));
}


void steamAppSearch(const QString &content)
{
    QDesktopServices::openUrl(QUrl("steam://openurl/https://store.steampowered.com/search/?term=" + content + "&ndl=1"));
}

void steamWebSearch(const QString &content)
{
    QDesktopServices::openUrl(QUrl("https://store.steampowered.com/search/?term=" + content + "&ndl=1"));
}

void steamDBSearch(const QString &content)
{
    QDesktopServices::openUrl(QUrl("https://steamdb.info/search/?a=all&q=" + content));
}

void steamUISearch(const QString &content)
{
    QDesktopServices::openUrl(QUrl("https://steamui.com/?search=" + content));
}

void bingSearch(const QString &content)
{
    QDesktopServices::openUrl(QUrl("https://cn.bing.com/search?q=" + content));
}

void baiduSearch(const QString &content)
{
    QDesktopServices::openUrl(QUrl("https://www.baidu.com/s?wd=" + content));
}



void explorerOpenPath(const QString &path)
{
    QProcess().startDetached("explorer.exe", { QDir::toNativeSeparators(path) });
}

void explorerSelectPath(const QString &path)
{
    QProcess().startDetached("explorer.exe", { "/select,", QDir::toNativeSeparators(path) });
}



QStringList splitStringLines(const QString &content, bool skipEmpty)
{
    static const QRegularExpression lineSplitRegex(R"(\R)", QRegularExpression::DontCaptureOption);

    return content.split(lineSplitRegex, skipEmpty ? Qt::SkipEmptyParts : Qt::KeepEmptyParts);
}

LuaInfo detectLuaInfo(const QStringList &content, const QString &defaultName, const QString &defaultAppID)
{
    static const QStringView gameNamePrefix    = u"-- 游戏名称:";
    static const QStringView gameAppIDPrefix   = u"-- AppID:";
    static const size_t gameNamePrefixLen  = gameNamePrefix.size();
    static const size_t gameAppIDPrefixLen = gameAppIDPrefix.size();

    LuaInfo info { false, false, defaultName, defaultAppID};

    for (auto it = content.constBegin(), end = content.constEnd(); (!info.hasName || !info.hasAppID) && it != end; ++it)
    {
        QString line = it->trimmed();

        if (line.isEmpty()) continue;

        if (!info.hasName && line.startsWith(gameNamePrefix))
        {
            info.name = line.mid(gameNamePrefixLen).trimmed();
            info.hasName = !info.name.isEmpty();
        }
        else if (!info.hasAppID && line.startsWith(gameAppIDPrefix))
        {
            info.appID = line.mid(gameAppIDPrefixLen).trimmed();
            info.hasAppID = !info.appID.isEmpty();
        }
    }

    return info;
}

LuaInfo detectLuaInfo(const QString &content, const QString &defaultName, const QString &defaultAppID)
{
    return detectLuaInfo(splitStringLines(content), defaultName, defaultAppID);
}

void formatLua(QStringList &content, const QString &name, const QString &appid)
{
    for (int i = content.size() - 1; i >= 0; --i)
    {
        content[i] = content[i].trimmed();

        // addappid 已足够，setManifestid 不需要，所以非 addappid 全删
        if (!content[i].startsWith("addappid"))
        {
            content.removeAt(i);
            continue;
        }
    }

    content = LuaInfo::toList(name, appid) + content;
}

void formatLua(QString &content, const QString &name, const QString &appid)
{
    QStringList lines = splitStringLines(content);
    formatLua(lines, name, appid);
    content = lines.join("\n");
}

void formatLua(QStringList &content)
{
    LuaInfo info = detectLuaInfo(content);

    formatLua(content, info.name, info.appID);
}

void formatLua(QString &content)
{
    LuaInfo info = detectLuaInfo(content);

    formatLua(content, info.name, info.appID);
}

QString formattedLua(const QString &content, const QString &name, const QString &appid)
{
    QStringList lines = splitStringLines(content);
    formatLua(lines, name, appid);
    return lines.join("\n");
}

QString formattedLua(const QString &content)
{
    LuaInfo info = detectLuaInfo(content);

    return formattedLua(content, info.name, info.appID);
}


FileEditErrorType renameFile(QFile *file, const QString &newName)
{
    FileEditErrorType errorType = NoError;

    QFileInfo oldFileInfo(file->fileName());
    QString newPath = oldFileInfo.path() + "/" + newName;

    if (newPath != oldFileInfo.filePath())
    {
        if (QFile::exists(newPath))
        {
            errorType |= NewNameExisted;
        }
        else
        {
            if (!file->rename(newPath))
            {
                errorType |= RenameFailed;
            }
        }
    }

    return errorType;
}

FileEditErrorType renameFile(const QString &filePath, const QString &newName)
{
    QFile file(filePath);

    return renameFile(&file, newName);
}

FileEditErrorType editLuaFile(const QString &filePath, const QString &gameName, const QString &gameAppID, bool shouleRename)
{
    FileEditErrorType errorType = NoError;

    QFile file(filePath);

    if (shouleRename)
    {
        errorType |= renameFile(&file, gameAppID + "." + QFileInfo(filePath).suffix());
    }

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return errorType | OpenFileFailed;
    }

    QString content = formattedLua(QTextStream(&file).readAll(), gameName, gameAppID);
    file.close();

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
    {
        return errorType | OpenFileFailed;
    }

    QTextStream out(&file);
    out.setEncoding(QStringConverter::Utf8);

    out << content;
    out.flush();

    file.close();

    return errorType;
}

QStringList getMimeDataPaths(const QMimeData *mime)
{
    QStringList paths;

    if (mime && mime->hasUrls())
    {
        for (const QUrl &url : mime->urls())
        {
            if (url.isLocalFile())
            {
                paths.append(url.toLocalFile());
            }
        }
    }

    return paths;
}
}

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
// 格式为 游戏名、appid、空行 各一行
QStringList LuaInfo::toList(const QString &name, const QString &appid)
{
    return QStringList({
        "-- 游戏名称: " + name,
        "-- AppID: " + appid,
        ""
    });
}

// 格式为 游戏名、appid、空行 各一行
QString LuaInfo::toString(const QString &name, const QString &appid)
{
    return LuaInfo::toList(name, appid).join('\n');
}

// 格式为 游戏名、appid、空行 各一行
QStringList LuaInfo::toList()
{
    return LuaInfo::toList(this->name, this->appID);
}
// 格式为 游戏名、appid、空行 各一行
QString LuaInfo::toString()
{
    return LuaInfo::toString(this->name, this->appID);
}



void closeSteam()
{
    QProcess process;
    process.start("taskkill", QStringList() << "/f" << "/im" << "Steam.exe");
    process.waitForFinished(3000);
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
    QProcess::startDetached("explorer.exe", { QDir::toNativeSeparators(path) });
}

void explorerSelectPath(const QString &path)
{
    QProcess::startDetached("explorer.exe", { "/select,", QDir::toNativeSeparators(path) });
}



QStringList splitStringLines(const QString &content)
{
    static const QRegularExpression lineSplitRegex(R"(\R)", QRegularExpression::DontCaptureOption);

    return content.split(lineSplitRegex);
}

LuaInfo findLuaInfo(const QString &content, const QString &defaultName, const QString &defaultAppID)
{
    static const QRegularExpression namePattern(R"pattern(--\s*游戏名称\s*:\s*(?<content>.+)$)pattern", QRegularExpression::MultilineOption);
    static const QRegularExpression appidPattern(R"pattern(--\s*AppID\s*:\s*(?<content>.+)$)pattern", QRegularExpression::MultilineOption);

    const QRegularExpressionMatch nameMatch(namePattern.match(content));
    const QRegularExpressionMatch appidMatch(appidPattern.match(content));

    bool hasNameMatch = nameMatch.hasMatch();
    bool hasAppidMatch = appidMatch.hasMatch();

    return { hasNameMatch, hasAppidMatch, (hasNameMatch ? nameMatch.captured("content").trimmed() : defaultName), (hasAppidMatch ? appidMatch.captured("content").trimmed() : defaultAppID) };
}

LuaInfo findLuaInfo(const QStringList &content, const QString &defaultName, const QString &defaultAppID)
{
    return findLuaInfo(content.join('\n'), defaultName, defaultAppID);
}



QString formattedLua(const QString &content, const QString &name, const QString &appid)
{
    static const QRegularExpression pattern(R"pattern((?<!\w)addappid\s*\(\s*(?<appid>\d+)\s*(?:,\s*(?<type>\d+)\s*(?:,\s*(?<quote>["'])(?<depot>\w+)\k<quote>\s*)?)?\))pattern");

    QStringList lines;
    QRegularExpressionMatchIterator it = pattern.globalMatch(content);
    while (it.hasNext())
    {
        QRegularExpressionMatch match = it.next();

        QString line = "addappid(" + QString::number(match.captured("appid").toInt());

        const QString type = match.captured("type");
        if (!type.isEmpty())
        {
            line.append(", " + QString::number(type.toInt()));

            const QString depot = match.captured("depot");
            if (!depot.isEmpty()) line.append(", \"" + depot + "\"");
        }

        line.append(")");

        lines.append(line);
    }

    return LuaInfo::toString(name, appid) + "\n" + lines.join('\n');
}

void formatLua(QStringList &content, const QString &name, const QString &appid)
{
    content = splitStringLines(formattedLua(content.join('\n'), name, appid));
}



FileEditErrorType renameFile(QFile *file, const QString &newName)
{
    FileEditErrorType errorType = NoError;

    QFileInfo oldFileInfo(file->fileName());
    QString newPath = oldFileInfo.path() + "/" + newName;

    if (QFileInfo(newPath) != oldFileInfo)
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

FileEditErrorType editLuaFile(const QString &filePath, const QString &gameName, const QString &gameAppID, bool shouldRename)
{
    FileEditErrorType errorType = NoError;

    QFile file(filePath);

    if (shouldRename) errorType |= renameFile(&file, gameAppID + "." + QFileInfo(filePath).suffix());

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return errorType | OpenFileFailed;

    QString content = formattedLua(QTextStream(&file).readAll(), gameName, gameAppID);
    file.close();

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) return errorType | OpenFileFailed;

    QTextStream(&file) << content;

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
                const QString path = url.toLocalFile();

                if (QFileInfo(path).isFile()) paths.append(path);
            }
        }
    }

    return paths;
}
}

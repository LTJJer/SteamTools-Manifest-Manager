#include "lua.hpp"

#include "constant.hpp"

#include <QDir>
#include <QFileInfo>



namespace Lua
{
/// begin class LuaInfo
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
    return LuaInfo::toList(name, appid).join('\n');
}



LuaInfo::LuaInfo(const QString &name, const QString &appid)
    : mv_name(name)
    , mv_appid(appid) {}

LuaInfo::LuaInfo()
    : LuaInfo("", "") {}



const QString &LuaInfo::getName() const
{
    return mv_name;
}

const QString &LuaInfo::getAppid() const
{
    return mv_appid;
}


void LuaInfo::setName(const QString &name)
{
    mv_name = name;
}

void LuaInfo::setAppid(const QString &appid)
{
    mv_appid = appid;
}



bool LuaInfo::hasName() const
{
    return !mv_name.isEmpty();
}

bool LuaInfo::hasAppid() const
{
    return !mv_appid.isEmpty();
}

QStringList LuaInfo::toList() const
{
    return toList(mv_name, mv_appid);
}

QString LuaInfo::toString() const
{
    return toString(mv_name, mv_appid);
}
/// end class LuaInfo



/// begin class LuaData
LuaData::LuaData(const QString &path, const QString &content, const QString &name, const QString &appid)
    : LuaInfo(name, appid)
    , mv_path(path)
    , mv_content(content) {}

LuaData::LuaData()
    : LuaData("", "", "", "") {}

LuaData &LuaData::operator=(const LuaInfo &other)
{
    setName(other.getName());
    setAppid(other.getAppid());

    return *this;
}



const QString &LuaData::getPath() const
{
    return mv_path;
}

const QString &LuaData::getContent() const
{
    return mv_content;
}

void LuaData::setPath(const QString &path)
{
    mv_path = path;
}

void LuaData::setContent(const QString &content)
{
    mv_content = content;
}



bool LuaData::hasPath() const
{
    return !mv_path.isEmpty();
}

bool LuaData::hasContent() const
{
    return !mv_content.isEmpty();
}

void LuaData::formatContent()
{
    Lua::formatLua(mv_content, mv_name, mv_appid);
}

void LuaData::formatContent(const QString &name, const QString &appid)
{
    Lua::formatLua(mv_content, name, appid);
}

void LuaData::setValidInfo(const LuaInfo &info)
{
    if (info.hasName()) this->setName(info.getName());
    if (info.hasAppid()) this->setAppid(info.getAppid());
}
/// end class LuaData



LuaInfo findLuaInfo(const QString &content, const QString &defaultName, const QString &defaultAppid)
{
    LuaInfo result;


    // mode detection
    static const QRegularExpression morrenusPattern(R"pattern(^-- (?<appid>\d+)'s Lua and Manifest Created by Morrenus\R-- (?<name>.*))pattern");
    static const QRegularExpression namePattern(R"pattern(--\h*游戏名称\h*:\h*(?<content>.+))pattern");
    static const QRegularExpression appidPattern(R"pattern(--\h*AppID\h*:\h*(?<content>.+))pattern");

    if (const QRegularExpressionMatch morrenusMatch(morrenusPattern.match(content)); morrenusMatch.hasMatch()) // Morrenus detection
    {
        result.setName(morrenusMatch.captured("name").trimmed());
        result.setAppid(morrenusMatch.captured("appid").trimmed());
    }
    else // default detection
    {
        if (const QRegularExpressionMatch nameMatch(namePattern.match(content)); nameMatch.hasMatch())
            result.setName(nameMatch.captured("content").trimmed());

        if (const QRegularExpressionMatch appidMatch(appidPattern.match(content)); appidMatch.hasMatch())
            result.setAppid(appidMatch.captured("content").trimmed());
    }


    // result
    if (!result.hasName() && !defaultName.isEmpty()) result.setName(defaultName);
    if (!result.hasAppid() && !defaultAppid.isEmpty()) result.setAppid(defaultAppid);

    return result;
}

LuaInfo findLuaInfo(const QStringList &content, const QString &defaultName, const QString &defaultAppid)
{
    return findLuaInfo(content.join('\n'), defaultName, defaultAppid);
}



QString formattedLua(const QString &content, const QString &name, const QString &appid)
{
    static const QRegularExpression pattern(R"pattern((?<!\w)addappid\h*\(\h*(?<appid>\d+)\h*(?:,\h*(?<type>\d+)\h*(?:,\h*(?<quote>["'])(?<depot>\w+)\k<quote>\h*)?)?\))pattern");

    QStringList lines;
    QRegularExpressionMatchIterator it = pattern.globalMatch(content);
    while (it.hasNext())
    {
        QRegularExpressionMatch match = it.next();

        QString line = "addappid(" + QString::number(match.captured("appid").toInt());

        if (const QString type = match.captured("type"); !type.isEmpty())
        {
            line.append(", " + QString::number(type.toInt()));

            if (const QString depot = match.captured("depot"); !depot.isEmpty())
                line.append(", \"" + depot + "\"");
        }

        line.append(")");

        lines.append(line);
    }

    return LuaInfo::toString(name, appid) + "\n" + lines.join('\n');
}

void formatLua(QString &content, const QString &name, const QString &appid)
{
    content = formattedLua(content, name, appid);
}

void formatLua(QStringList &content, const QString &name, const QString &appid)
{
    content = FunctionLib::splitStringLines(formattedLua(content.join('\n'), name, appid));
}



FunctionLib::FileEditErrorType addLuaFile(
    const QString &targetDir, QString content,
    QString name, QString appid, QString fileName,
    bool shouldFormat, bool intelligentName, bool intelligentAppid, bool fileNameUseAppid,
    LuaData *luaData, std::function<bool (const QString &filePath)> confirmOverwrite)
{
    // 数据准备
    if (intelligentName || intelligentAppid)
    {
        LuaInfo info = findLuaInfo(content);

        if (intelligentName && info.hasName()) name = info.getName();
        if (intelligentAppid && info.hasAppid()) appid = info.getAppid();
    }


    if (shouldFormat) formatLua(content, name, appid);
    else content.prepend(QString("-- 游戏名称: %1\n-- AppID: %2\n\n").arg(name, appid));


    if (fileNameUseAppid) fileName = appid;
    fileName = QDir(targetDir).filePath((fileName.endsWith(QString(".%1").arg(Constant::luaEnabledSuffix)) || fileName.endsWith(QString(".%1").arg(Constant::luaDisabledSuffix))) ? fileName : (fileName + "." + Constant::luaEnabledSuffix));


    if (luaData) *luaData = LuaData(fileName, content, name, appid);



    // 文件写入
    QFile file(fileName);
    if (file.exists() && (!confirmOverwrite || !confirmOverwrite(fileName))) return FunctionLib::NewNameExisted; // 默认不覆盖
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) return FunctionLib::OpenFileFailed;

    QTextStream(&file) << content;

    file.close();



    return FunctionLib::NoError;
}



FunctionLib::FileEditErrorType editLuaFile(QFile *file, const QString &name, const QString &appid, bool shouldRename)
{
    FunctionLib::FileEditErrorType errorType = FunctionLib::NoError;

    if (shouldRename) errorType |= FunctionLib::renameFile(file, appid + "." + QFileInfo(*file).suffix());

    if (!file->open(QIODevice::ReadOnly | QIODevice::Text)) return errorType | FunctionLib::OpenFileFailed;

    QString content = formattedLua(QTextStream(file).readAll(), name, appid);
    file->close();

    if (!file->open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) return errorType | FunctionLib::OpenFileFailed;

    QTextStream(file) << content;

    file->close();

    return errorType;
}

FunctionLib::FileEditErrorType editLuaFile(QString *filePath, const QString &name, const QString &appid, bool shouldRename)
{
    QFile file(*filePath);
    auto ret = editLuaFile(&file, name, appid, shouldRename);
    if (shouldRename) *filePath = file.fileName();
    return ret;
}

FunctionLib::FileEditErrorType editLuaFile(const QString &filePath, const QString &name, const QString &appid, bool shouldRename)
{
    QFile file(filePath);
    return editLuaFile(&file, name, appid, shouldRename);
}
}

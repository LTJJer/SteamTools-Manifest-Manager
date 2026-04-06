#pragma once

#include "functionlib.hpp"

#include <QStringList>



namespace Lua
{
class LuaInfo
{
public:
    static QStringList toList(const QString &name, const QString &appid);
    static QString toString(const QString &name, const QString &appid);

public:
    LuaInfo(const QString &name, const QString &appid);
    LuaInfo();

    virtual ~LuaInfo() = default;

public:
    const QString &getName() const;
    const QString &getAppid() const;

    void setName(const QString &name);
    void setAppid(const QString &appid);

public:
    bool hasName() const;
    bool hasAppid() const;

    virtual QStringList toList() const;
    virtual QString toString() const;

protected:
    QString mv_name;
    QString mv_appid;
};

class LuaData : public LuaInfo
{
public:
    LuaData(const QString &path, const QString &content, const QString &name, const QString &appid);
    LuaData();

    virtual ~LuaData() = default;


    LuaData &operator=(const LuaInfo &other);

public:
    const QString &getPath() const;
    const QString &getContent() const;

    void setPath(const QString &path);
    void setContent(const QString &content);

    void formatContent();
    void formatContent(const QString &name, const QString &appid);

public:
    bool hasPath() const;
    bool hasContent() const;

    void setValidInfo(const LuaInfo &info);

protected:
    QString mv_path;
    QString mv_content;
};



LuaInfo findLuaInfo(const QString &content, const QString &defaultName = "", const QString &defaultAppid = "");
LuaInfo findLuaInfo(const QStringList &content, const QString &defaultName = "", const QString &defaultAppid = "");

QString formattedLua(const QString &content, const QString &name, const QString &appid);
void formatLua(QString &content, const QString &name, const QString &appid);
void formatLua(QStringList &content, const QString &name, const QString &appid);

FunctionLib::FileEditErrorType addLuaFile(
    const QString &targetDir, QString content,
    QString name = "", QString appid = "", QString fileName = "",
    bool shouldFormat = true, bool intelligentName = true, bool intelligentAppid = true, bool fileNameUseAppid = true,
    LuaData *luaData = nullptr, std::function<bool (const QString &filePath)> confirmOverwrite = nullptr);

FunctionLib::FileEditErrorType editLuaFile(QFile *file, const QString &name, const QString &appid, bool shouldRename = true);
FunctionLib::FileEditErrorType editLuaFile(QString *filePath, const QString &gameName, const QString &gameAppid, bool shouldRename = true);
FunctionLib::FileEditErrorType editLuaFile(const QString &filePath, const QString &gameName, const QString &gameAppid, bool shouldRename = true);
}

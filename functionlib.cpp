#include "functionlib.hpp"

#include "constant.hpp"
#include "settingsdialog.hpp"

#include <QProcess>
#include <QTimer>
#include <QDesktopServices>
#include <QUrl>
#include <QDir>
#include <QRegularExpression>
#include <QUrlQuery>
#include <QFileInfo>
#include <QApplication>
#include <QSettings>
#include <qt_windows.h>



namespace FunctionLib
{
QString toWindowsPath(QString path)
{
    path = QDir::toNativeSeparators(path);
    if (path.size() >= 2 && path[1] == ':') path[0] = path[0].toUpper();

    return path;
}



FileEditErrorType renameFile(QFile *file, const QString &newName)
{
    FileEditErrorType errorType = NoError;

    QFileInfo oldFileInfo(file->fileName());
    QString newPath = QDir(oldFileInfo.path()).filePath("./" + newName);

    if (QFileInfo(newPath) != oldFileInfo)
    {
        if (QFile::exists(newPath))
        {
            errorType |= NewNameExisted;
            errorType |= RenameFailed;
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

    QTimer::singleShot(interval, [steamFilePath]()
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
    QUrl url("steam://openurl/https://store.steampowered.com/search/");
    url.setQuery(QUrlQuery({{ "term", content }, { "ndl", "1" }}));

    QDesktopServices::openUrl(url);
}

void steamWebSearch(const QString &content)
{
    QUrl url("https://store.steampowered.com/search/");
    url.setQuery(QUrlQuery({{ "term", content }, { "ndl", "1" }}));

    QDesktopServices::openUrl(url);
}

void steamDBSearch(const QString &content)
{
    QUrl url("https://steamdb.info/search/");
    url.setQuery(QUrlQuery({{ "a", "all" }, { "q", content }}));

    QDesktopServices::openUrl(url);
}

void steamUISearch(const QString &content)
{
    QUrl url("https://steamui.com/");
    url.setQuery(QUrlQuery({{ "search", content }}));

    QDesktopServices::openUrl(url);
}

void bingSearch(const QString &content)
{
    QUrl url("https://cn.bing.com/search");
    url.setQuery(QUrlQuery({{ "q", content }}));

    QDesktopServices::openUrl(url);
}

void baiduSearch(const QString &content)
{
    QUrl url("https://www.baidu.com/s");
    url.setQuery(QUrlQuery({{ "wd", content }}));

    QDesktopServices::openUrl(url);
}



void explorerOpenPath(const QString &path)
{
    QDesktopServices::openUrl(QUrl::fromLocalFile(path));
}

void explorerSelectPath(const QString &path)
{
    QProcess::startDetached("explorer.exe", { "/select,", QDir::toNativeSeparators(path) }); // select 指令和路径需要分开为两个参数
}



QStringList splitStringLines(const QString &content)
{
    static const QRegularExpression lineSplitRegex(R"(\R)", QRegularExpression::DontCaptureOption);

    return content.split(lineSplitRegex);
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



bool isSystemThemeLight()
{
    return QSettings("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize", QSettings::NativeFormat).value("AppsUseLightTheme", 1) == 1;
}

bool isThemeLight()
{
    int mode = SettingsDialog::getSettings(SettingsDialog::Style).toInt();

    return (mode == 0) ? isSystemThemeLight() : (mode == 1 ? true : false);
}

QString getSystemThemeStyleSheet()
{
    return isSystemThemeLight() ? Constant::getLightThemeStyleSheet() : Constant::getDarkThemeStyleSheet();
}

QString getThemeStyleSheet()
{
    return isThemeLight() ? Constant::getLightThemeStyleSheet() : Constant::getDarkThemeStyleSheet();
}

void applyThemeStyle(QWidget *widget)
{
    widget->setStyleSheet(getThemeStyleSheet());

    const HWND hwnd = reinterpret_cast<HWND>(widget->winId());
    ::SetWindowPos(hwnd, nullptr, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
}
}

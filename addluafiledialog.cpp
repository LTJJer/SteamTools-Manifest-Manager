#include "addluafiledialog.hpp"
#include "ui_addluafiledialog.h"

#include "functionlib.hpp"

#include <QFileDialog>
#include <QStandardPaths>
#include <QFile>
#include <QFileInfo>
#include <QMessageBox>



AddLuaFileDialog::AddLuaFileDialog(const QString &luaDir, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AddLuaFileDialog)
{
    ui->setupUi(this);

    this->luaDir = luaDir;
}

AddLuaFileDialog::~AddLuaFileDialog()
{
    delete ui;
}



void AddLuaFileDialog::dragEnterEvent(QDragEnterEvent *event)
{
    if (!FunctionLib::getMimeDataPaths(event->mimeData()).isEmpty()) event->acceptProposedAction();
}

void AddLuaFileDialog::dropEvent(QDropEvent *event)
{
    const QString path = FunctionLib::getMimeDataPaths(event->mimeData()).constFirst();

    if (path.isEmpty() || !QFile::exists(path))
    {
        event->ignore();
        return;
    }

    this->import(path);

    event->acceptProposedAction();
}

void AddLuaFileDialog::import(const QString &path)
{
    if (path.isEmpty()) return;


    // 文件
    const QString fileBaseName = QFileInfo(path).baseName();

    ui->le_FileName->setText(fileBaseName);


    // 内容
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::critical(this, "失败", "打开文件失败");
        return;
    }

    const QString content = QString::fromUtf8(file.readAll());

    ui->ContentEdit->setPlainText(content);


    // 标记
    const FunctionLib::LuaInfo info = FunctionLib::detectLuaInfo(content, "", fileBaseName);

    ui->le_Name->setText(info.name);
    ui->le_Appid->setText(info.appID);

    ui->chk_AutoName->setChecked(false);
    ui->chk_AutoAppid->setChecked(false);
    ui->chk_AutoFileName->setChecked(false);
}



void AddLuaFileDialog::on_importButton_clicked()
{
    this->import(QFileDialog::getOpenFileName(
        this,
        "选择导入的文件",
        QStandardPaths::writableLocation(QStandardPaths::DesktopLocation),
        "Lua 文件 (*.lua);;文本文件 (*.txt);;Lua 和文本文件 (*.lua *.txt);;所有文件 (*.*)"
        ));
}

void AddLuaFileDialog::on_CancelButton_clicked()
{
    this->close();
}

void AddLuaFileDialog::on_OKButton_clicked()
{
    // 数据准备
    QStringList          content { FunctionLib::splitStringLines(ui->ContentEdit->toPlainText()) };
    FunctionLib::LuaInfo info;

    bool aName  = ui->chk_AutoName->isChecked();
    bool aAppid = ui->chk_AutoAppid->isChecked();
    bool aFile  = ui->chk_AutoFileName->isChecked();

    if (aName || aAppid || aFile) info = FunctionLib::detectLuaInfo(content);



    QString gameName     = (aName && info.hasName) ? info.name : ui->le_Name->text();
    QString appid        = (aAppid && info.hasAppID) ? info.appID : ui->le_Appid->text();

    QString fileBaseName = !aFile ? ui->le_FileName->text() : appid;
    QString filePath     = QDir(this->luaDir).filePath((fileBaseName.endsWith(QString(".%1").arg(enabledSuffix)) || fileBaseName.endsWith(QString(".%1").arg(disabledSuffix))) ? fileBaseName : (QString("%1.%2").arg(fileBaseName, enabledSuffix)));



    // 格式化
    if (ui->chk_ShouldFormat->isChecked())
    {
        FunctionLib::formatLua(content, gameName, appid);
    }
    else
    {
        content.prepend("");
        content.prepend("-- AppID: " + appid);
        content.prepend("-- 游戏名称: " + gameName);
    }


    // 写入
    QFile file(filePath);

    if (file.exists() && (QMessageBox::question(this, "该文件名已存在", "该文件名已存在\n是否覆盖？") == QMessageBox::No)) return;

    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
    {
        QMessageBox::critical(this, "失败", "文件打开失败");
        return;
    }

    QTextStream stream(&file);
    stream.setEncoding(QStringConverter::Utf8);

    stream << content.join("\n");

    file.close();

    emit addingFinished(filePath, gameName, appid, content);

    this->close();
}

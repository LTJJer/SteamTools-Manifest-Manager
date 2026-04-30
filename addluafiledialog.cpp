#include "addluafiledialog.hpp"
#include "ui_addluafiledialog.h"

#include "lua.hpp"
#include "constant.hpp"
#include "functionlib.hpp"

#include <QFileDialog>
#include <QStandardPaths>
#include <QFile>
#include <QFileInfo>
#include <QMessageBox>
#include <QValidator>
#include <QRegularExpressionValidator>
#include <QTextStream>



AddLuaFileDialog::AddLuaFileDialog(const QString &luaDir, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AddLuaFileDialog)
    , mv_luaDir(luaDir)
{
    ui->setupUi(this);

    ui->le_FileName->setValidator(new QRegularExpressionValidator(Constant::pathPattern, this));
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
    const QStringList paths = FunctionLib::getMimeDataPaths(event->mimeData());

    if (paths.isEmpty())
    {
        event->ignore();
        return;
    }

    for (const QString &path : paths)
    {
        this->import(path, true);
    }

    event->acceptProposedAction();
}



void AddLuaFileDialog::import(const QString &path, bool append)
{
    if (path.isEmpty()) return;



    // 内容
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::critical(this, "失败", "打开文件失败");
        return;
    }

    const QString content = QTextStream(&file).readAll();
    file.close();

    if (append) ui->txt_Content->setPlainText(ui->txt_Content->toPlainText() + "\n\n" + content);
    else ui->txt_Content->setPlainText(content);


    // 标记
    const QFileInfo fileInfo = QFileInfo(path);

    const Lua::LuaInfo info = Lua::findLuaInfo(content, fileInfo.dir().dirName(), fileInfo.baseName());

    ui->le_Name->setText(info.getName());
    ui->le_Appid->setText(info.getAppid());
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

// 文件基础名允许为空
void AddLuaFileDialog::on_OKButton_clicked()
{
    Lua::LuaData data;

    const FunctionLib::FileEditErrorType error = Lua::addLuaFile(
        mv_luaDir, ui->txt_Content->toPlainText(),
        ui->le_Name->text(), ui->le_Appid->text(), ui->le_FileName->text(),
        ui->chk_ShouldFormat->isChecked(),
        ui->le_Name->text().isEmpty(), ui->le_Appid->text().isEmpty(), ui->le_FileName->text().isEmpty(),
        &data, [this](const QString &filePath) -> bool
        {
            return QMessageBox::question(this, "该文件名已存在", QString("该文件名已存在：\n%1\n\n是否覆盖？").arg(FunctionLib::toWindowsPath(filePath))) == QMessageBox::Yes;
        });



    if (error & FunctionLib::NewNameExisted) return;

    if (error)
    {
        QMessageBox::critical(this, "添加时发生错误", FunctionLib::generateFileEditErrorString(error));
        return;
    }

    emit addingFinished(error, data);

    this->close();
}

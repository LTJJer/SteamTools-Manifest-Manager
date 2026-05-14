#include "exportdialog.hpp"
#include "ui_exportdialog.h"

#include "constant.hpp"

#include <QSettings>



ExportDialog::ExportDialog(const QString &sourceLuaPath, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ExportDialog)
    , mv_sourceLuaPath(sourceLuaPath)
{
    ui->setupUi(this);

    ui->btng_FileNameMode->setId(ui->rdo_Normal_ChangeFileName_Appid, FileNameChangeMode::Appid);
    ui->btng_FileNameMode->setId(ui->rdo_Normal_ChangeFileName_Name, FileNameChangeMode::Name);
    ui->btng_ContentProcessMode->setId(ui->rdo_Normal_ContentProcess_Format, ContentProcessMode::Format);
    ui->btng_ContentProcessMode->setId(ui->rdo_Normal_ContentProcess_FormatNoInfo, ContentProcessMode::FormatNoInfo);
    ui->btng_ContentProcessMode->setId(ui->rdo_Normal_ContentProcess_OnlyInfo, ContentProcessMode::OnlyInfo);

    // 用户配置
    {
        ui->tab_ExportMode->setCurrentIndex(getSettings(ExportMode).toInt());

        // 普通
        {
            ui->gbx_Normal_ChangeFileName->setChecked(getSettings(Normal_isChangeFileName).toBool());
            ui->btng_FileNameMode->button(getSettings(Normal_FileNameMode).toInt())->setChecked(true);

            ui->gbx_Normal_ContentProcess->setChecked(getSettings(Normal_isContentProcess).toBool());
            ui->btng_ContentProcessMode->button(getSettings(Normal_ContentProcessMode).toInt())->setChecked(true);

            ui->le_Normal_Dir->setText(getSettings(Normal_Dir).toString());
        }

        // 合并
        {
            ui->gbx_Merge_WriteInfo->setChecked(getSettings(Merge_isWriteInfo).toBool());

            ui->le_Merge_Path->setText(getSettings(Merge_Path).toString());
        }

        ui->chk_OpenFolder->setChecked(getSettings(OpenFolder).toBool());
    }
}

ExportDialog::~ExportDialog()
{
    delete ui;
}



QString ExportDialog::getSettingsName(SettingsItems item)
{
    switch (item)
    {
    case SettingsItems::ExportMode:
        return "Export/ExportMode";

    case SettingsItems::Normal_isChangeFileName:
        return "Export/Normal/isChangeFileName";

    case SettingsItems::Normal_FileNameMode:
        return "Export/Normal/FileNameMode";

    case SettingsItems::Normal_isContentProcess:
        return "Export/Normal/isContentProcess";

    case SettingsItems::Normal_ContentProcessMode:
        return "Export/Normal/ContentProcessMode";

    case SettingsItems::Normal_Dir:
        return "Export/Normal/Dir";

    case SettingsItems::Merge_isWriteInfo:
        return "Export/Merge/isWriteInfo";

    case SettingsItems::Merge_Path:
        return "Export/Merge/Path";

    case SettingsItems::OpenFolder:
        return "Export/OpenFolder";

    default:
        return "";
    }
}

QVariant ExportDialog::getSettingsDefaultValue(SettingsItems item)
{
    switch (item)
    {
    case SettingsItems::ExportMode:
        return 0;

    case SettingsItems::Normal_isChangeFileName:
        return true;

    case SettingsItems::Normal_FileNameMode:
        return FileNameChangeMode::Name;

    case SettingsItems::Normal_isContentProcess:
        return false;

    case SettingsItems::Normal_ContentProcessMode:
        return ContentProcessMode::Format;

    case SettingsItems::Normal_Dir:
        return "";

    case SettingsItems::Merge_isWriteInfo:
        return false;

    case SettingsItems::Merge_Path:
        return "";

    case SettingsItems::OpenFolder:
        return true;

    default:
        return QVariant();
    }
}

QVariant ExportDialog::getSettings(const SettingsItems item)
{
    return Constant::getSettings().value(getSettingsName(item), getSettingsDefaultValue(item));
}



void ExportDialog::setSettings(const SettingsItems item, const QVariant &value)
{
    Constant::getSettings().setValue(getSettingsName(item), value);
    emit settingsChanged(item, value);
}



void ExportDialog::on_tab_ExportMode_currentChanged(int index)
{
    setSettings(ExportMode, index);
}

void ExportDialog::on_gbx_Normal_ChangeFileName_toggled(bool arg1)
{
    setSettings(Normal_isChangeFileName, arg1);
}

void ExportDialog::on_btng_FileNameMode_idClicked(int id)
{
    setSettings(Normal_FileNameMode, id);
}

void ExportDialog::on_gbx_Normal_ContentProcess_toggled(bool arg1)
{
    setSettings(Normal_isContentProcess, arg1);
}

void ExportDialog::on_btng_ContentProcessMode_idClicked(int id)
{
    setSettings(Normal_ContentProcessMode, id);
}

void ExportDialog::on_le_Normal_Dir_textChanged(const QString &arg1)
{
    setSettings(Normal_Dir, arg1);
}

void ExportDialog::on_gbx_Merge_WriteInfo_toggled(bool arg1)
{
    setSettings(Merge_isWriteInfo, arg1);
}

void ExportDialog::on_le_Merge_Path_textChanged(const QString &arg1)
{
    setSettings(Merge_Path, arg1);
}

void ExportDialog::on_chk_OpenFolder_clicked(bool checked)
{
    setSettings(OpenFolder, checked);
}





#include "exportdialog.hpp"
#include "ui_exportdialog.h"

#include "constant.hpp"



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
        ui->tab_ExportMode->setCurrentIndex(Constant::getSettings().value("Export/ExportMode", 0).toInt());

        // 普通
        {
            ui->gbx_Normal_ChangeFileName->setChecked(Constant::getSettings().value("Export/Normal/isChangeFileName", true).toBool());
            ui->btng_FileNameMode->button(Constant::getSettings().value("Export/Normal/FileNameMode", FileNameChangeMode::Name).value<FileNameChangeMode>())->setChecked(true);

            ui->gbx_Normal_ContentProcess->setChecked(Constant::getSettings().value("Export/Normal/isContentProcess", false).toBool());
            ui->btng_ContentProcessMode->button(Constant::getSettings().value("Export/Normal/ContentProcessMode", ContentProcessMode::Format).value<ContentProcessMode>())->setChecked(true);

            ui->le_Normal_Dir->setText(Constant::getSettings().value("Export/Normal/Dir", "").toString());
        }

        // 合并
        {
            ui->gbx_Merge_WriteInfo->setChecked(Constant::getSettings().value("Export/Merge/isWriteInfo", false).toBool());

            ui->le_Merge_Path->setText(Constant::getSettings().value("Export/Merge/Path", "").toString());
        }

        ui->chk_OpenFolder->setChecked(Constant::getSettings().value("Export/OpenFolder", true).toBool());
    }
}

ExportDialog::~ExportDialog()
{
    delete ui;
}


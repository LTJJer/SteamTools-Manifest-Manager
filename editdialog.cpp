#include "editdialog.hpp"
#include "ui_editdialog.h"

#include "lua.hpp"

#include <QMessageBox>
#include <QDir>



EditDialog::EditDialog(const QString &filePath, const QString &gameName, const QString &gameAppid, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::EditDialog)
    , mv_path(filePath)
    , mv_name(gameName)
    , mv_appid(gameAppid)
{
    ui->setupUi(this);

    ui->GameNameEdit->setText(mv_name);
    ui->AppIDEdit->setText(mv_appid);
}

EditDialog::~EditDialog()
{
    delete ui;
}


void EditDialog::on_CancelButton_clicked()
{
    this->close();
}

void EditDialog::on_OKButton_clicked()
{
    const bool update = ui->UpdateFileNameCheckBox->isChecked();

    const QString name = ui->GameNameEdit->text().isEmpty() ? mv_name : ui->GameNameEdit->text();
    const QString appid = ui->AppIDEdit->text().isEmpty() ? mv_appid : ui->AppIDEdit->text();

    const FunctionLib::FileEditErrorType error = Lua::editLuaFile(&mv_path, name, appid, update);

    if (error) QMessageBox::warning(this, "编辑时发生错误", FunctionLib::generateFileEditErrorString(error));

    emit editFinished(
        error,
        mv_path,
        (error & FunctionLib::OpenFileFailed) ? mv_name  : name,
        (error & FunctionLib::OpenFileFailed) ? mv_appid : appid
    );

    if (!error) this->close();
}

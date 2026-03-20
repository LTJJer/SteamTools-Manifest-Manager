#include "editdialog.hpp"
#include "ui_editdialog.h"

#include <QMessageBox>
#include <QDir>



EditDialog::EditDialog(const QString &filePath, const QString &gameName, const QString &gameAppid, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::EditDialog)
{
    ui->setupUi(this);

    this->path = filePath;
    this->appid = gameAppid;
    this->name = gameName;

    ui->GameNameEdit->setText(this->name);
    ui->AppIDEdit->setText(this->appid);
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
    bool update = ui->UpdateFileNameCheckBox->isChecked();

    const QString name = ui->GameNameEdit->text().isEmpty() ? this->name : ui->GameNameEdit->text();
    const QString appid = ui->AppIDEdit->text().isEmpty() ? this->appid : ui->AppIDEdit->text();

    FunctionLib::FileEditErrorType error = FunctionLib::editLuaFile(path, name, appid, update);

    if (error)
    {
        QMessageBox::warning(this, "编辑时发生错误", FunctionLib::generateFileEditErrorString(error));
    }

    QFileInfo fi(this->path);

    emit editFinished(
        error,
        (update && !(error & FunctionLib::NewNameExisted) && !(error & FunctionLib::RenameFailed)) ? (QDir(fi.path()).filePath(appid) + "." + fi.suffix()) :  this->path,
        (error & FunctionLib::OpenFileFailed) ? this->name  : name,
        (error & FunctionLib::OpenFileFailed) ? this->appid : appid
    );

    if (!error) this->close();
}

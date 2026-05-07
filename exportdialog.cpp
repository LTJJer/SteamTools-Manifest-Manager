#include "exportdialog.hpp"
#include "ui_exportdialog.h"



ExportDialog::ExportDialog(const QString &sourceLuaPath, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ExportDialog)
    , mv_sourceLuaPath(sourceLuaPath)
{
    ui->setupUi(this);
}

ExportDialog::~ExportDialog()
{
    delete ui;
}


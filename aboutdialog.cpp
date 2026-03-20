#include "aboutdialog.hpp"
#include "ui_aboutdialog.h"

#include <QDesktopServices>
#include <QUrl>



AboutDialog::AboutDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
}

AboutDialog::~AboutDialog()
{
    delete ui;
}



void AboutDialog::on_btn_LT_JJGitHub_clicked()
{
    QDesktopServices::openUrl(QUrl("https://github.com/LTJJer"));
}

void AboutDialog::on_btn_LT_JJBilibili_clicked()
{
    QDesktopServices::openUrl(QUrl("https://space.bilibili.com/3546693838572464"));
}

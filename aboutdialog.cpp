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



void AboutDialog::on_btn_LT_JJ_GitHub_clicked()
{
    QDesktopServices::openUrl(QUrl("https://github.com/LTJJer"));
}

void AboutDialog::on_btn_LT_JJ_Bilibili_clicked()
{
    QDesktopServices::openUrl(QUrl("https://space.bilibili.com/3546693838572464"));
}

void AboutDialog::on_btn_Donate_LT_JJ_Ifdian_clicked()
{
    QDesktopServices::openUrl(QUrl("https://www.ifdian.net/a/LT_JJ"));
}

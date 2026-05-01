#include "aboutdialog.hpp"
#include "ui_aboutdialog.h"

#include "wechatdonatedialog.hpp"

#include <QDesktopServices>
#include <QUrl>



AboutDialog::AboutDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AboutDialog)
{
    ui->setupUi(this);

    ui->lbl_Version->setText(QString("(STMM v%1)").arg(APP_VERSION));
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

void AboutDialog::on_btn_Donate_LT_JJ_WeChat_clicked()
{
    WeChatDonateDialog dlg(this);
    dlg.exec();
}

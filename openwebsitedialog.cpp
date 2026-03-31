#include "openwebsitedialog.hpp"
#include "ui_openwebsitedialog.h"

#include <QDesktopServices>
#include <QUrl>



OpenWebsiteDialog::OpenWebsiteDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::OpenWebsiteDialog)
{
    ui->setupUi(this);
}

OpenWebsiteDialog::~OpenWebsiteDialog()
{
    delete ui;
}


void OpenWebsiteDialog::on_btn_Tool_SteamTools_clicked()
{
    QDesktopServices::openUrl(QUrl("https://www.steamtools.net"));
}

void OpenWebsiteDialog::on_btn_Manifest_Assiw_clicked()
{
    QDesktopServices::openUrl(QUrl("https://steam.assiw.xyz"));
}

void OpenWebsiteDialog::on_btn_Manifest_SteamManifestHub_2_clicked()
{
    QDesktopServices::openUrl(QUrl("https://ssmg4.github.io/ManifestHubDownloader"));
}

void OpenWebsiteDialog::on_btn_Manifest_ManifestHub_clicked()
{
    QDesktopServices::openUrl(QUrl("https://github.com/SteamAutoCracks/ManifestHub"));
}

void OpenWebsiteDialog::on_btn_Data_SteamDB_clicked()
{
    QDesktopServices::openUrl(QUrl("https://steamdb.info"));
}

void OpenWebsiteDialog::on_btn_Data_SteamUI_clicked()
{
    QDesktopServices::openUrl(QUrl("https://steamui.com"));
}

void OpenWebsiteDialog::on_btn_Community_3ACommunity_clicked()
{
    QDesktopServices::openUrl(QUrl("https://3a.lol"));
}

void OpenWebsiteDialog::on_btn_Manifest_SteamManifestDownloader_clicked()
{
    QDesktopServices::openUrl(QUrl("https://manifest.youngzm.com"));
}

void OpenWebsiteDialog::on_pushButton_clicked()
{
    QDesktopServices::openUrl(QUrl("https://manifest.steam.run"));
}

void OpenWebsiteDialog::on_btn_Community_Caigamer_clicked()
{
    QDesktopServices::openUrl(QUrl("https://caigamer.cn"));
}


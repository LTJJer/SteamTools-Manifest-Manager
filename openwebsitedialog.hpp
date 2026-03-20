#pragma once

#include <QDialog>



namespace Ui { class OpenWebsiteDialog; }



class OpenWebsiteDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OpenWebsiteDialog(QWidget *parent = nullptr);
    ~OpenWebsiteDialog();

private slots:
    void on_btn_Tool_SteamTools_clicked();
    void on_btn_Manifest_Assiw_clicked();
    void on_btn_Manifest_SteamManifestHub_2_clicked();
    void on_btn_Manifest_ManifestHub_clicked();
    void on_btn_Data_SteamDB_clicked();
    void on_btn_Data_SteamUI_clicked();
    void on_btn_Forum_SteamToolsOld_clicked();
    void on_btn_Forum_SteamToolsNew_clicked();

private:
    Ui::OpenWebsiteDialog *ui;
};

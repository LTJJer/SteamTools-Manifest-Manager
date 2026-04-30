#pragma once

#include <QMainWindow>
#include <QListWidget>
#include <qevent.h>
#include <QMimeData>



namespace Ui { class MainWindow; }



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // QWidget interface
protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private slots:
    void refresh();

    void editItem(QListWidgetItem *item);

    void filterItems();

    void runItem(QListWidgetItem *item);



    void on_lst_Items_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

    void on_btn_Edit_clicked();
    void on_btn_DeleteSelectedItem_clicked();
    void on_btn_ToggleLuaEnabled_clicked();
    void on_btn_AddLuaFile_clicked();
    void on_btn_OpenFile_clicked();
    void on_btn_OpenPath_clicked();
    void on_btn_FormatAll_clicked();
    void on_btn_CopyAppid_clicked();
    void on_btn_CopyGameName_clicked();
    void on_btn_CopyInfo_clicked();
    void on_btn_CopyFileContent_clicked();
    void on_btn_Run_clicked();
    void on_btn_OpenShop_clicked();
    void on_btn_RestartSteam_clicked();
    void on_btn_CloseSteam_clicked();
    void on_btn_Website_clicked();
    void on_btn_Search_clicked();
    void on_btn_Settings_clicked();
    void on_btn_About_clicked();

private:
    void addItem(const QString &path, const QString &name, const QString &appid, bool select = true, bool sort = true, bool filter = true);

private:
    Ui::MainWindow *ui;

    const QString mv_LuaDir;
    const QString mv_steamExe;
};

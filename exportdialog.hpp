#pragma once

#include <QDialog>
#include <QVariant>



namespace Ui { class ExportDialog; }



class ExportDialog : public QDialog
{
    Q_OBJECT

private:
    enum FileNameChangeMode
    {
        Appid,
        Name,
    };
    enum ContentProcessMode
    {
        Format,
        FormatNoInfo,
        OnlyInfo,
    };
    enum SettingsItems
    {
        ExportMode,
        Normal_isChangeFileName,
        Normal_FileNameMode,
        Normal_isContentProcess,
        Normal_ContentProcessMode,
        Normal_Dir,
        Merge_isWriteInfo,
        Merge_Path,
        OpenFolder,
    };

signals:
    void settingsChanged(ExportDialog::SettingsItems item, QVariant value);

public:
    explicit ExportDialog(const QString &sourceLuaPath, QWidget *parent = nullptr);
    ~ExportDialog();

private:
    static QString getSettingsName(SettingsItems item);
    static QVariant getSettingsDefaultValue(SettingsItems item);
    static QVariant getSettings(const SettingsItems item);

private:
    void setSettings(const SettingsItems item, const QVariant &value);

private slots:
    void on_tab_ExportMode_currentChanged(int index);
    void on_gbx_Normal_ChangeFileName_toggled(bool arg1);
    void on_btng_FileNameMode_idClicked(int id);
    void on_gbx_Normal_ContentProcess_toggled(bool arg1);
    void on_btng_ContentProcessMode_idClicked(int id);
    void on_le_Normal_Dir_textChanged(const QString &arg1);
    void on_gbx_Merge_WriteInfo_clicked(bool checked);
    void on_le_Merge_Path_textChanged(const QString &arg1);
    void on_chk_OpenFolder_clicked(bool checked);

    void on_gbx_Merge_WriteInfo_toggled(bool arg1);

private:
    Ui::ExportDialog *ui;

    const QString mv_sourceLuaPath;
};

#pragma once

#include <QDialog>



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

public:
    explicit ExportDialog(const QString &sourceLuaPath, QWidget *parent = nullptr);
    ~ExportDialog();

private:
    Ui::ExportDialog *ui;

    const QString mv_sourceLuaPath;
};

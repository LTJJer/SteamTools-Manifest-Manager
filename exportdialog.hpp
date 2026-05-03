#pragma once

#include <QDialog>



namespace Ui { class ExportDialog; }



class ExportDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ExportDialog(QWidget *parent = nullptr);
    ~ExportDialog();

private:
    Ui::ExportDialog *ui;
};

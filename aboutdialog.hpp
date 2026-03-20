#pragma once

#include <QDialog>



namespace Ui { class AboutDialog; }



class AboutDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AboutDialog(QWidget *parent = nullptr);
    ~AboutDialog();

private slots:
    void on_btn_LT_JJGitHub_clicked();
    void on_btn_LT_JJBilibili_clicked();

private:
    Ui::AboutDialog *ui;
};

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
    void on_btn_LT_JJ_GitHub_clicked();
    void on_btn_LT_JJ_Bilibili_clicked();

    void on_btn_Donate_LT_JJ_Ifdian_clicked();

private:
    Ui::AboutDialog *ui;
};

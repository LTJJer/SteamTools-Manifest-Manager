#pragma once

#include <QDialog>



namespace Ui { class SearchDialog; }



class SearchDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SearchDialog(const QString &defaultContent, QWidget *parent = nullptr);
    ~SearchDialog();



    enum SearchWay : int
    {
        SteamApp,
        SteamWeb,
        SteamDB,
        SteamUI,
        Bing,
        Baidu
    };

private slots:
    void on_btn_Cancel_clicked();

    void on_btn_Search_clicked();

private:
    Ui::SearchDialog *ui;
};

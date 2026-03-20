#include "searchdialog.hpp"
#include "ui_searchdialog.h"

#include "functionlib.hpp"



SearchDialog::SearchDialog(const QString &defaultContent, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SearchDialog)
{
    ui->setupUi(this);

    ui->btng_SearchWeb->setId(ui->rdo_SteamApp , SteamApp);
    ui->btng_SearchWeb->setId(ui->rdo_SteamWeb , SteamWeb);
    ui->btng_SearchWeb->setId(ui->rdo_SteamDB  , SteamDB);
    ui->btng_SearchWeb->setId(ui->rdo_SteamUI  , SteamUI);
    ui->btng_SearchWeb->setId(ui->rdo_Bing     , Bing);
    ui->btng_SearchWeb->setId(ui->rdo_Baidu    , Baidu);

    ui->le_Content->setText(defaultContent);
}

SearchDialog::~SearchDialog()
{
    delete ui;
}


void SearchDialog::on_btn_Cancel_clicked()
{
    this->close();
}

void SearchDialog::on_btn_Search_clicked()
{
    SearchWay way = static_cast<SearchWay>(ui->btng_SearchWeb->checkedId());
    const QString content = ui->le_Content->text();

    switch (way)
    {
    case SteamApp:
        FunctionLib::steamAppSearch(content);
        break;

    case SteamWeb:
        FunctionLib::steamWebSearch(content);
        break;

    case SteamDB:
        FunctionLib::steamDBSearch(content);
        break;

    case SteamUI:
        FunctionLib::steamUISearch(content);
        break;

    case Bing:
        FunctionLib::bingSearch(content);
        break;

    case Baidu:
        FunctionLib::baiduSearch(content);
        break;

    default:
        break;
    }

    this->close();
}

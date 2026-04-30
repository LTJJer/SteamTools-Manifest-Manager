#include "settingsdialog.hpp"
#include "ui_settingsdialog.h"

#include "constant.hpp"

#include <QSettings>
#include <QStyledItemDelegate>
#include <QStyleHints>



QString SettingsDialog::getSettingsName(SettingsItems item)
{
    switch (item)
    {
    case Style:
        return "Display/Style";

    default:
        return "";
    }
}

QVariant SettingsDialog::getSettingsDefaultValue(SettingsItems item)
{
    switch (item)
    {
    case Style:
        return 0;

    default:
        return QVariant();
    }
}

QVariant SettingsDialog::getSettings(const SettingsItems item)
{
    return Constant::getSettings().value(getSettingsName(item), getSettingsDefaultValue(item));
}



SettingsDialog::SettingsDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);

    ui->cbo_Style->setItemDelegate(new QStyledItemDelegate(this));



    // 设置
    {
        QSettings settings = Constant::getSettings();

        ui->cbo_Style->setCurrentIndex(getSettings(Style).toInt());
    }
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}



void SettingsDialog::setSettings(const SettingsItems item, const QVariant &value)
{
    Constant::getSettings().setValue(getSettingsName(item), value);
    emit settingsChenged(item, value);
}

void SettingsDialog::advanceUpdateStyle(int mode)
{
    switch (mode)
    {
    case 0:
        QApplication::styleHints()->setColorScheme(Qt::ColorScheme::Unknown);
        break;

    case 1:
        QApplication::styleHints()->setColorScheme(Qt::ColorScheme::Light);
        break;

    case 2:
        QApplication::styleHints()->setColorScheme(Qt::ColorScheme::Dark);
        break;

    default:
        break;
    }
    QApplication::processEvents(); // 让我们的史山 Qt 喘口气

    setSettings(Style, mode);
}



void SettingsDialog::on_lst_Category_currentRowChanged(int currentRow)
{
    ui->swgt_Settings->setCurrentIndex(currentRow);
}

void SettingsDialog::on_btn_Complate_clicked()
{
    this->close();
}


void SettingsDialog::on_cbo_Style_currentIndexChanged(int index)
{
    this->advanceUpdateStyle(index);
}

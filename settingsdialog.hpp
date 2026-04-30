#pragma once



#include <QDialog>



namespace Ui { class SettingsDialog; }



class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    enum SettingsItems
    {
        Style // 0：自动；1：浅色；2：深色
    };

signals:
    void settingsChenged(SettingsDialog::SettingsItems item, QVariant value);

public:
    static QString getSettingsName(SettingsItems item);
    static QVariant getSettingsDefaultValue(SettingsItems item);

    static QVariant getSettings(const SettingsItems item);

public:
    explicit SettingsDialog(QWidget *parent = nullptr);
    ~SettingsDialog();

public:
    void setSettings(const SettingsItems item, const QVariant &value);

    void advanceUpdateStyle(int mode);

private slots:
    void on_lst_Category_currentRowChanged(int currentRow);
    void on_btn_Complate_clicked();
    // 设置项
    void on_cbo_Style_currentIndexChanged(int index);

private:
    Ui::SettingsDialog *ui;
};

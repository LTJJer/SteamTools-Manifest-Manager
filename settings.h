#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>

namespace Ui {
class Settings;
}

enum class Theme { Auto, Light, Dark };

class Settings : public QDialog
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = nullptr);
    ~Settings();

    static void applyTheme(Theme theme, QWidget *widget = nullptr);
    static Theme currentTheme();

private slots:
    void on_cancelButton_clicked();
    void on_okButton_clicked();
    void on_autoThemeRadio_toggled(bool checked);
    void on_lightThemeRadio_toggled(bool checked);
    void on_darkThemeRadio_toggled(bool checked);
    void on_sidebarList_currentRowChanged(int currentRow);

private:
    Ui::Settings *ui;
    Theme selectedTheme;
    
    void setupThemeUI();
    static void setWindowsTitleBarColor(QWidget *widget, bool isDark);
    static QString getDarkThemeStylesheet();
    static QString getLightThemeStylesheet();
};

#endif // SETTINGS_H

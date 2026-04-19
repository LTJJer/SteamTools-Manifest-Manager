#include "settings.h"
#include "ui_settings.h"
#include <QSettings>
#include <QApplication>
#include <QStyleFactory>
#include <QPalette>

#ifdef Q_OS_WIN
#include <windows.h>
#include <dwmapi.h>

#ifndef DWMWA_USE_IMMERSIVE_DARK_MODE
#define DWMWA_USE_IMMERSIVE_DARK_MODE 20
#endif

#ifndef DWMWA_CAPTION_COLOR
#define DWMWA_CAPTION_COLOR 35
#endif
#endif

Settings::Settings(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Settings)
    , selectedTheme(Theme::Auto)
{
    ui->setupUi(this);
    setupThemeUI();
    applyTheme(selectedTheme, this);
}

Settings::~Settings()
{
    delete ui;
}

void Settings::setupThemeUI()
{
    QSettings settings("SteamTools", "ManifestManager");
    QString themeStr = settings.value("theme", "auto").toString();
    
    if (themeStr == "light") {
        selectedTheme = Theme::Light;
        ui->lightThemeRadio->setChecked(true);
    } else if (themeStr == "dark") {
        selectedTheme = Theme::Dark;
        ui->darkThemeRadio->setChecked(true);
    } else {
        selectedTheme = Theme::Auto;
        ui->autoThemeRadio->setChecked(true);
    }
}

Theme Settings::currentTheme()
{
    QSettings settings("SteamTools", "ManifestManager");
    QString themeStr = settings.value("theme", "auto").toString();
    
    if (themeStr == "light") {
        return Theme::Light;
    } else if (themeStr == "dark") {
        return Theme::Dark;
    }
    return Theme::Auto;
}

void Settings::applyTheme(Theme theme, QWidget *widget)
{
    QWidget *targetWidget = widget ? widget : QApplication::activeWindow();
    if (!targetWidget) return;

    bool isDark = false;
    
    if (theme == Theme::Auto) {
#ifdef Q_OS_WIN
        QSettings registrySettings("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize", QSettings::NativeFormat);
        isDark = registrySettings.value("AppsUseLightTheme", 1).toInt() == 0;
#else
        QPalette palette = QApplication::style()->standardPalette();
        isDark = palette.color(QPalette::Window).value() < 128;
#endif
    } else {
        isDark = (theme == Theme::Dark);
    }

    QString stylesheet = isDark ? getDarkThemeStylesheet() : getLightThemeStylesheet();
    targetWidget->setStyleSheet(stylesheet);
    setWindowsTitleBarColor(targetWidget, isDark);
}

void Settings::setWindowsTitleBarColor(QWidget *widget, bool isDark)
{
#ifdef Q_OS_WIN
    if (!widget) return;
    
    HWND hwnd = reinterpret_cast<HWND>(widget->winId());
    if (!hwnd) return;

    BOOL useDarkMode = isDark ? TRUE : FALSE;
    DwmSetWindowAttribute(hwnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &useDarkMode, sizeof(useDarkMode));
#endif
}

QString Settings::getDarkThemeStylesheet()
{
    return R"(
QWidget {
    margin: 0;
    padding: 0;
    background-color: #202020;
    color: #E0E0E0;
    outline: none;
}

QWidget:disabled {
    color: #505050;
}

QPushButton {
    padding: 6px 12px;
    min-height: 20px;
    background-color: #282828;
    border: 1px solid #404040;
    border-radius: 4px;
}

QPushButton:hover {
    background-color: #383838;
    color: #D0D0D0;
}

QPushButton:pressed {
    background-color: #1c1c1c;
    color: #C0C0C0;
}

QPushButton:disabled {
    background-color: #252525;
    color: #505050;
}

QRadioButton, QCheckBox {
    spacing: 8px;
    min-height: 24px;
}

QRadioButton::indicator, QCheckBox::indicator {
    background-color: #303030;
    border: 1px solid #404040;
}

QRadioButton::indicator:hover, QCheckBox::indicator:hover {
    background-color: #383838;
    border-color: #505050;
}

QRadioButton::indicator:pressed, QCheckBox::indicator:pressed {
    background-color: #282828;
    border-color: #5078A0;
}

QRadioButton::indicator:checked, QCheckBox::indicator:checked {
    background-color: #5078A0;
    border-color: #5078A0;
}

QRadioButton::indicator:checked:hover, QCheckBox::indicator:checked:hover {
    background-color: #6088B0;
    border-color: #6088B0;
}

QRadioButton::indicator:checked:pressed, QCheckBox::indicator:checked:pressed {
    background-color: #406890;
    border-color: #406890;
}

QRadioButton::indicator:disabled, QCheckBox::indicator:disabled {
    background-color: #282828;
    border-color: #383838;
}

QRadioButton::indicator:disabled:checked, QCheckBox::indicator:disabled:checked {
    background-color: #383838;
    border-color: #484848;
}

QRadioButton::indicator {
    width: 8px;
    height: 8px;
    border-radius: 4px;
}

QCheckBox::indicator {
    width: 18px;
    height: 18px;
    border-radius: 2px;
}

QGroupBox {
    border: 1px solid #404040;
    border-radius: 4px;
    margin-top: 20px;
}

QGroupBox::title {
    subcontrol-origin: margin;
    subcontrol-position: top center;
    padding: 0 6px;
}

QListWidget {
    background-color: #303030;
    border: 1px solid #404040;
    border-radius: 4px;
    selection-background-color: #383838;
    selection-color: #E0E0E0;
    alternate-background-color: #353535;
}

QListWidget::item:hover {
    background-color: #353535;
}

QListWidget::item:pressed {
    background-color: #282828;
}

QListWidget::item:selected:hover {
    background-color: #282828;
    border: 1px solid #404040;
}

QTabBar::tab {
    padding: 6px 12px;
    min-height: 20px;
    margin-right: 2px;
    background-color: #282828;
    border: 1px solid #404040;
    border-bottom: none;
    border-top-left-radius: 4px;
    border-top-right-radius: 4px;
}

QTabBar::tab:hover {
    background-color: #383838;
    color: #D0D0D0;
}

QTabBar::tab:selected {
    background-color: #383838;
    color: #E0E0E0;
    border-top: 1px solid #5078A0;
}

QTabBar::tab:pressed {
    background-color: #181818;
    color: #C0C0C0;
}

QTabBar::tab:disabled {
    background-color: #252525;
    color: #505050;
    border-color: #383838;
}

QTabWidget::pane {
    border: 1px solid #404040;
    border-top: none;
    margin-top: -1px;
}

QLineEdit, QDateTimeEdit, QSpinBox, QDoubleSpinBox, QComboBox, QTextEdit, QPlainTextEdit {
    min-height: 20px;
    background-color: #303030;
    border: 1px solid #404040;
    border-radius: 4px;
    color: #D0D0D0;
    padding: 4px 8px;
}

QComboBox {
    padding-right: 28px;
}

QComboBox:editable {
    padding-right: 8px;
}

QLineEdit:hover, QDateTimeEdit:hover, QSpinBox:hover, QDoubleSpinBox:hover, QComboBox:hover, QComboBox:editable:hover, QTextEdit:hover, QPlainTextEdit:hover {
    background-color: #383838;
    border-color: #505050;
}

QLineEdit:focus, QDateTimeEdit:focus, QSpinBox:focus, QDoubleSpinBox:focus, QComboBox:focus, QComboBox:editable:focus, QTextEdit:focus, QPlainTextEdit:focus {
    border-color: #5078A0;
}

QLineEdit:disabled, QDateTimeEdit:disabled, QSpinBox:disabled, QDoubleSpinBox:disabled, QComboBox:disabled, QComboBox:editable:disabled, QTextEdit:disabled, QPlainTextEdit:disabled {
    background-color: #282828;
    border-color: #383838;
    color: #505050;
}

QLineEdit:read-only, QDateTimeEdit:read-only, QSpinBox:read-only, QDoubleSpinBox:read-only, QTextEdit:read-only, QPlainTextEdit:read-only {
    background-color: #282828;
    color: #A0A0A0;
}

QComboBox::drop-down {
    subcontrol-origin: padding;
    subcontrol-position: top right;
    width: 20px;
    border-top-right-radius: 4px;
    border-bottom-right-radius: 4px;
}

QComboBox::drop-down:hover {
    background-color: #383838;
}

QComboBox::drop-down:pressed {
    background-color: #282828;
}

QComboBox::drop-down:disabled {
    background-color: #252525;
}

QComboBox QAbstractItemView {
    background-color: #303030;
    border: 1px solid #5078A0;
    selection-background-color: #383838;
    selection-color: #E0E0E0;
}

QComboBox QAbstractItemView::item:hover {
    background-color: #353535;
}

QComboBox QAbstractItemView::item:selected {
    background-color: #383838;
}

QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {
    background: none;
}

QScrollBar:vertical {
    background: #303030;
    width: 8px;
}

QScrollBar::handle:vertical {
    background: #585858;
    min-height: 20px;
}

QScrollBar::handle:vertical:hover {
    background: #686868;
}

QScrollBar::handle:vertical:pressed {
    background: #5078A0;
}

QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
    height: 0;
}
)";
}

QString Settings::getLightThemeStylesheet()
{
    return R"(
QWidget {
    margin: 0;
    padding: 0;
    background-color: #FFFFFF;
    color: #202020;
    outline: none;
}

QWidget:disabled {
    color: #909090;
}

QPushButton {
    padding: 6px 12px;
    min-height: 20px;
    background-color: #F0F0F0;
    border: 1px solid #D0D0D0;
    border-radius: 4px;
}

QPushButton:hover {
    background-color: #E0E0E0;
    color: #101010;
}

QPushButton:pressed {
    background-color: #D0D0D0;
    color: #000000;
}

QPushButton:disabled {
    background-color: #E8E8E8;
    color: #909090;
}

QRadioButton, QCheckBox {
    spacing: 8px;
    min-height: 24px;
}

QRadioButton::indicator, QCheckBox::indicator {
    background-color: #F5F5F5;
    border: 1px solid #C0C0C0;
}

QRadioButton::indicator:hover, QCheckBox::indicator:hover {
    background-color: #E8E8E8;
    border-color: #A0A0A0;
}

QRadioButton::indicator:pressed, QCheckBox::indicator:pressed {
    background-color: #D8D8D8;
    border-color: #4080C0;
}

QRadioButton::indicator:checked, QCheckBox::indicator:checked {
    background-color: #4080C0;
    border-color: #4080C0;
}

QRadioButton::indicator:checked:hover, QCheckBox::indicator:checked:hover {
    background-color: #5090D0;
    border-color: #5090D0;
}

QRadioButton::indicator:checked:pressed, QCheckBox::indicator:checked:pressed {
    background-color: #3070B0;
    border-color: #3070B0;
}

QRadioButton::indicator:disabled, QCheckBox::indicator:disabled {
    background-color: #E8E8E8;
    border-color: #D0D0D0;
}

QRadioButton::indicator:disabled:checked, QCheckBox::indicator:disabled:checked {
    background-color: #D8D8D8;
    border-color: #C0C0C0;
}

QRadioButton::indicator {
    width: 8px;
    height: 8px;
    border-radius: 4px;
}

QCheckBox::indicator {
    width: 18px;
    height: 18px;
    border-radius: 2px;
}

QGroupBox {
    border: 1px solid #D0D0D0;
    border-radius: 4px;
    margin-top: 20px;
}

QGroupBox::title {
    subcontrol-origin: margin;
    subcontrol-position: top center;
    padding: 0 6px;
}

QListWidget {
    background-color: #F8F8F8;
    border: 1px solid #D0D0D0;
    border-radius: 4px;
    selection-background-color: #E0E0E0;
    selection-color: #202020;
    alternate-background-color: #F0F0F0;
}

QListWidget::item:hover {
    background-color: #F0F0F0;
}

QListWidget::item:pressed {
    background-color: #E8E8E8;
}

QListWidget::item:selected:hover {
    background-color: #E8E8E8;
    border: 1px solid #C0C0C0;
}

QTabBar::tab {
    padding: 6px 12px;
    min-height: 20px;
    margin-right: 2px;
    background-color: #F0F0F0;
    border: 1px solid #D0D0D0;
    border-bottom: none;
    border-top-left-radius: 4px;
    border-top-right-radius: 4px;
}

QTabBar::tab:hover {
    background-color: #E0E0E0;
    color: #101010;
}

QTabBar::tab:selected {
    background-color: #E0E0E0;
    color: #202020;
    border-top: 1px solid #4080C0;
}

QTabBar::tab:pressed {
    background-color: #D0D0D0;
    color: #000000;
}

QTabBar::tab:disabled {
    background-color: #E8E8E8;
    color: #909090;
    border-color: #D0D0D0;
}

QTabWidget::pane {
    border: 1px solid #D0D0D0;
    border-top: none;
    margin-top: -1px;
}

QLineEdit, QDateTimeEdit, QSpinBox, QDoubleSpinBox, QComboBox, QTextEdit, QPlainTextEdit {
    min-height: 20px;
    background-color: #F8F8F8;
    border: 1px solid #D0D0D0;
    border-radius: 4px;
    color: #202020;
    padding: 4px 8px;
}

QComboBox {
    padding-right: 28px;
}

QComboBox:editable {
    padding-right: 8px;
}

QLineEdit:hover, QDateTimeEdit:hover, QSpinBox:hover, QDoubleSpinBox:hover, QComboBox:hover, QComboBox:editable:hover, QTextEdit:hover, QPlainTextEdit:hover {
    background-color: #FFFFFF;
    border-color: #A0A0A0;
}

QLineEdit:focus, QDateTimeEdit:focus, QSpinBox:focus, QDoubleSpinBox:focus, QComboBox:focus, QComboBox:editable:focus, QTextEdit:focus, QPlainTextEdit:focus {
    border-color: #4080C0;
}

QLineEdit:disabled, QDateTimeEdit:disabled, QSpinBox:disabled, QDoubleSpinBox:disabled, QComboBox:disabled, QComboBox:editable:disabled, QTextEdit:disabled, QPlainTextEdit:disabled {
    background-color: #F0F0F0;
    border-color: #D0D0D0;
    color: #909090;
}

QLineEdit:read-only, QDateTimeEdit:read-only, QSpinBox:read-only, QDoubleSpinBox:read-only, QTextEdit:read-only, QPlainTextEdit:read-only {
    background-color: #F0F0F0;
    color: #606060;
}

QComboBox::drop-down {
    subcontrol-origin: padding;
    subcontrol-position: top right;
    width: 20px;
    border-top-right-radius: 4px;
    border-bottom-right-radius: 4px;
}

QComboBox::drop-down:hover {
    background-color: #E0E0E0;
}

QComboBox::drop-down:pressed {
    background-color: #D0D0D0;
}

QComboBox::drop-down:disabled {
    background-color: #E8E8E8;
}

QComboBox QAbstractItemView {
    background-color: #F8F8F8;
    border: 1px solid #4080C0;
    selection-background-color: #E0E0E0;
    selection-color: #202020;
}

QComboBox QAbstractItemView::item:hover {
    background-color: #F0F0F0;
}

QComboBox QAbstractItemView::item:selected {
    background-color: #E0E0E0;
}

QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {
    background: none;
}

QScrollBar:vertical {
    background: #F8F8F8;
    width: 8px;
}

QScrollBar::handle:vertical {
    background: #A0A0A0;
    min-height: 20px;
}

QScrollBar::handle:vertical:hover {
    background: #909090;
}

QScrollBar::handle:vertical:pressed {
    background: #4080C0;
}

QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
    height: 0;
}
)";
}

void Settings::on_cancelButton_clicked()
{
    close();
}

void Settings::on_okButton_clicked()
{
    QSettings settings("SteamTools", "ManifestManager");
    QString themeStr;
    
    if (selectedTheme == Theme::Light) {
        themeStr = "light";
    } else if (selectedTheme == Theme::Dark) {
        themeStr = "dark";
    } else {
        themeStr = "auto";
    }
    
    settings.setValue("theme", themeStr);
    
    if (parentWidget()) {
        applyTheme(selectedTheme, parentWidget());
    }
    
    close();
}

void Settings::on_autoThemeRadio_toggled(bool checked)
{
    if (checked) {
        selectedTheme = Theme::Auto;
        applyTheme(selectedTheme, this);
    }
}

void Settings::on_lightThemeRadio_toggled(bool checked)
{
    if (checked) {
        selectedTheme = Theme::Light;
        applyTheme(selectedTheme, this);
    }
}

void Settings::on_darkThemeRadio_toggled(bool checked)
{
    if (checked) {
        selectedTheme = Theme::Dark;
        applyTheme(selectedTheme, this);
    }
}

void Settings::on_sidebarList_currentRowChanged(int currentRow)
{
    ui->stackedWidget->setCurrentIndex(currentRow);
}

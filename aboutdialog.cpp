#include "aboutdialog.hpp"
#include "ui_aboutdialog.h"

#include <QDesktopServices>
#include <QUrl>

#ifdef Q_OS_WIN
#include <windows.h>
#endif



AboutDialog::AboutDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AboutDialog)
{
    ui->setupUi(this);

    // 禁止最大化、禁止调整大小
    setWindowFlags(Qt::Dialog | Qt::WindowTitleHint | Qt::WindowCloseButtonHint | Qt::WindowSystemMenuHint | Qt::MSWindowsFixedSizeDialogHint);

#ifdef Q_OS_WIN
    HWND hwnd = (HWND)winId();
    if (hwnd) {
        LONG_PTR style = GetWindowLongPtrW(hwnd, GWL_STYLE);
        style &= ~(WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_THICKFRAME);
        SetWindowLongPtrW(hwnd, GWL_STYLE, style);
    }
#endif
}

AboutDialog::~AboutDialog()
{
    delete ui;
}



void AboutDialog::on_btn_LT_JJ_GitHub_clicked()
{
    QDesktopServices::openUrl(QUrl("https://github.com/LTJJer"));
}

void AboutDialog::on_btn_LT_JJ_Bilibili_clicked()
{
    QDesktopServices::openUrl(QUrl("https://space.bilibili.com/3546693838572464"));
}

void AboutDialog::on_btn_Donate_LT_JJ_Ifdian_clicked()
{
    QDesktopServices::openUrl(QUrl("https://www.ifdian.net/a/LT_JJ"));
}
